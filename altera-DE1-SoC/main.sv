module main (
    input  wire        CLOCK_50,
    input  wire [1:0]  KEY,      
     
    // HPS DDR3 Pins
    output wire [14:0] HPS_DDR3_ADDR,
    output wire [2:0]  HPS_DDR3_BA,
    output wire        HPS_DDR3_CAS_N,
    output wire        HPS_DDR3_CKE,
    output wire        HPS_DDR3_CK_N,
    output wire        HPS_DDR3_CK_P,
    output wire        HPS_DDR3_CS_N,
    output wire [3:0]  HPS_DDR3_DM,
    inout  wire [31:0] HPS_DDR3_DQ,
    inout  wire [3:0]  HPS_DDR3_DQS_N,
    inout  wire [3:0]  HPS_DDR3_DQS_P,
    output wire        HPS_DDR3_ODT,
    output wire        HPS_DDR3_RAS_N,
    output wire        HPS_DDR3_RESET_N,
    input  wire        HPS_DDR3_RZQ,
    output wire        HPS_DDR3_WE_N,

    // HPS Minimum I/O
    output wire        HPS_UART_TX,
    input  wire        HPS_UART_RX,

    // FPGA LEDs
    output wire [9:0]  LEDR
);

      wire [31:0] hps_to_fpga_data;
      wire [31:0] hps_to_fpga_data_rom;
      
      wire [31:0] imem_data;
      wire [12:0] imem_addr;
      
      wire [31:0] dmem_wdata;
      wire [12:0] dmem_addr;
      wire [31:0] dmem_rdata;

      wire dmem_we;
      
      assign imem_data = hps_to_fpga_data_rom;
      assign dmem_rdata = hps_to_fpga_data;
      
      wire clk    = CLOCK_50;
      wire areset = ~KEY[1];
      wire enable = 1'b1;

      // ── 2-Cycle State Control ──────────────────────────────────────
      logic execute_cycle;

      always_ff @(posedge clk or posedge areset) begin
        if (areset) begin
          execute_cycle <= 1'b0;
        end else if (enable) begin
          execute_cycle <= ~execute_cycle;
        end
      end

      // ── PC ────────────────────────────────────────────────────────
      wire [31:0] address_to_load;
      wire        load_address;
      wire [31:0] PC_Address;
      wire        halt;

      // Gate the PC updates so they only happen on the Execute cycle
      wire actual_pc_enable = enable & execute_cycle;
      wire actual_load_address = load_address & execute_cycle;

      PC pc (
            .clk            (clk),
            .PCEnable       (actual_pc_enable),
            .areset         (areset),
            .address_to_load(address_to_load),
            .load_address   (actual_load_address),
            .PC_Address     (PC_Address),
            .halt           (halt)
      ); 
      
      // ── Memory Interface (ROM Address) ────────────────────────────
      // Feed the current PC directly to the ROM. 
      // It uses bits [14:2] to match your 13-bit word-address format.
      assign imem_addr = PC_Address[14:2];

      // ── Control Unit ──────────────────────────────────────────────
      wire [1:0] ALUOps;
      wire ALUSrc, MemWrite, RegWrite;
      wire MemRead, MemtoReg, Branch, Jump;

      ControlUnit cu (
            .instruction  (imem_data),
            .ALUOps       (ALUOps),
            .ALUSrc       (ALUSrc),
            .MemWrite     (MemWrite),
            .RegWrite     (RegWrite),
            .MemRead      (MemRead),
            .MemtoReg     (MemtoReg),
            .Branch       (Branch),
            .Jump         (Jump),
            .halt_detected(halt)
      );
      
      // ── ImmGen ────────────────────────────────────────────────────
      wire [31:0] imm_out;

      ImmGen immgen (
            .instruction(imem_data),
            .Branch     (Branch),
            .Jump       (Jump),
            .MemWrite   (MemWrite),
            .imm_out    (imm_out)
      );

      // ── RegFile ───────────────────────────────────────────────────
      wire [31:0] ReadRegOut1, ReadRegOut2;
      wire [31:0] WriteData;

      // Gate register writes
      wire actual_reg_write = RegWrite & execute_cycle;

      RegFile regfile (
            .clk           (clk),
            .RegWriteEnable(actual_reg_write),
            .areset        (areset),
            .DATA          (WriteData),
            .ReadReg1Addr  (imem_data[19:15]),
            .ReadReg2Addr  (imem_data[24:20]),
            .WriteAddr     (imem_data[11:7]),
            .ReadRegOut1   (ReadRegOut1),
            .ReadRegOut2   (ReadRegOut2)
      );

      // ── ALU Control ───────────────────────────────────────────────
      wire [3:0] ALUCU_OUT;

      ALUControlUnit alucu (
            .funct3     (imem_data[14:12]),
            .funct7_bit5(imem_data[30]),
            .ALUOp      (ALUOps),
            .ALUCU_OUT  (ALUCU_OUT)
      );

      // ── ALU ───────────────────────────────────────────────────────
      wire [31:0] ALU_B_Mux;
      wire        ALUZero;
      wire [31:0] ALUOut;

      assign ALU_B_Mux = ALUSrc ? imm_out : ReadRegOut2;  // ← ALUSrc=1 uses immediate

      ALU alu (
            .x      (ReadRegOut1),
            .y      (ALU_B_Mux),
            .ALUops (ALUCU_OUT),
            .ALUZero(ALUZero),
            .ALUOut (ALUOut)
      );

      // ── PC Next & Branching ───────────────────────────────────────
      wire [31:0] BranchAddr = PC_Address + imm_out;
      wire [31:0] JumpAddr = PC_Address + imm_out;

      wire BranchTaken = ALUZero ^ imem_data[12];

      assign load_address = Jump | (Branch & BranchTaken);
      assign address_to_load = Jump ? JumpAddr : BranchAddr;

      // ── RegFile Write Data Mux ────────────────────────────────────
      assign WriteData = MemtoReg ? dmem_rdata : ALUOut;
      
      // ── Memory Interface (RAM Output) ─────────────────────────────
      // Gate memory writes
      assign dmem_we    = MemWrite & execute_cycle;
      assign dmem_addr  = ALUOut[14:2]; // Keep your 13-bit conversion
      assign dmem_wdata = ReadRegOut2;

    // ── HPS SoC Instantiation ───────────────────────────────────────
    soc_system u0 (
        .clk_clk                           (CLOCK_50),
        .reset_reset_n                     (KEY[0]),

        // DDR3 Interface
        .memory_mem_a                      (HPS_DDR3_ADDR),
        .memory_mem_ba                     (HPS_DDR3_BA),
        .memory_mem_ck                     (HPS_DDR3_CK_P),
        .memory_mem_ck_n                   (HPS_DDR3_CK_N),
        .memory_mem_cke                    (HPS_DDR3_CKE),
        .memory_mem_cs_n                   (HPS_DDR3_CS_N),
        .memory_mem_ras_n                  (HPS_DDR3_RAS_N),
        .memory_mem_cas_n                  (HPS_DDR3_CAS_N),
        .memory_mem_we_n                   (HPS_DDR3_WE_N),
        .memory_mem_reset_n                (HPS_DDR3_RESET_N),
        .memory_mem_dq                     (HPS_DDR3_DQ),
        .memory_mem_dqs                    (HPS_DDR3_DQS_P),
        .memory_mem_dqs_n                  (HPS_DDR3_DQS_N),
        .memory_mem_odt                    (HPS_DDR3_ODT),
        .memory_mem_dm                     (HPS_DDR3_DM),
        .memory_oct_rzqin                  (HPS_DDR3_RZQ),

        .hps_io_hps_io_uart0_inst_RX       (HPS_UART_RX),
        .hps_io_hps_io_uart0_inst_TX       (HPS_UART_TX),

        // First on-chip memory (Linux writes, FPGA reads/writes)
        .onchip_memory2_0_s2_address       (dmem_addr),
        .onchip_memory2_0_s2_chipselect    (1'b1),
        .onchip_memory2_0_s2_clken         (1'b1),
        .onchip_memory2_0_s2_write         (dmem_we),
        .onchip_memory2_0_s2_writedata     (dmem_wdata),
        .onchip_memory2_0_s2_readdata      (hps_to_fpga_data),
        .onchip_memory2_0_s2_byteenable    (4'hF),

        // Second on-chip memory (Linux writes, FPGA reads only)
        .onchip_memory2_1_s2_address       (imem_addr),
        .onchip_memory2_1_s2_chipselect    (1'b1),
        .onchip_memory2_1_s2_clken         (1'b1),
        .onchip_memory2_1_s2_write         (1'b0),  // FPGA never writes
        .onchip_memory2_1_s2_readdata      (hps_to_fpga_data_rom),
        .onchip_memory2_1_s2_byteenable    (4'hF)
    );
      
   assign LEDR[9:3] = 7'b0; // Fixed bit width to match 7 bits (9 down to 3)
   assign LEDR[1:0] = 2'b0;
   
   assign LEDR[2] = (halt == 1'b1);

endmodule
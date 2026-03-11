module main (
    input  logic        clk,
    input  logic        areset,
    input  logic        enable,
    // ROM input
    output logic [31:0] imem_addr,
    input  logic [31:0] imem_data,

    // RAM input
    output logic        dmem_clk,
    output logic [31:0] dmem_addr,
    output logic [31:0] dmem_wdata,
    output logic        dmem_we,
    input  logic [31:0] dmem_rdata
);

  // ── 2-Cycle State Control ──────────────────────────────────────
  // 0 = Fetch (ROM is fetching based on PC_Address)
  // 1 = Execute (Instruction is valid, ALU runs, states update)
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

  // The PC is only allowed to change at the end of the Execute cycle
  wire        actual_pc_enable = enable & execute_cycle;
  wire        actual_load_address = load_address & execute_cycle;

  PC pc (
      .clk            (clk),
      .PCEnable       (actual_pc_enable),
      .areset         (areset),
      .address_to_load(address_to_load),
      .load_address   (actual_load_address),
      .PC_Address     (PC_Address),
      .halt           (halt)
  );

  // ── Memory Interface (ROM) ────────────────────────────────────
  // Give the ROM the address immediately. It will be ready by the Execute cycle.
  assign imem_addr = PC_Address >> 2;

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

  // Only allow register writes during the Execute cycle
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

  assign ALU_B_Mux = ALUSrc ? imm_out : ReadRegOut2;

  ALU alu (
      .x      (ReadRegOut1),
      .y      (ALU_B_Mux),
      .ALUops (ALUCU_OUT),
      .ALUZero(ALUZero),
      .ALUOut (ALUOut)
  );

  // ── PC Next & Branching ───────────────────────────────────────
  // Since the PC is stalled, we calculate branches directly from PC_Address
  wire [31:0] BranchAddr = PC_Address + imm_out;
  wire [31:0] JumpAddr = PC_Address + imm_out;

  wire BranchTaken = ALUZero ^ imem_data[12];

  assign load_address = Jump | (Branch & BranchTaken);
  assign address_to_load = Jump ? JumpAddr : BranchAddr;

  // ── RegFile Write Data Mux ────────────────────────────────────
  assign WriteData = MemtoReg ? dmem_rdata : ALUOut;

  // ── Memory Interface (RAM) ────────────────────────────────────
  assign dmem_clk   = clk;
  // Only allow memory writes during the Execute cycle
  assign dmem_we    = MemWrite & execute_cycle;
  assign dmem_addr  = ALUOut;
  assign dmem_wdata = ReadRegOut2;

endmodule

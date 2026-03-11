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

  // ── PC ────────────────────────────────────────────────────────
  wire [31:0] address_to_load;
  wire        load_address;
  wire [31:0] PC_Address;
  wire        halt;

  PC pc (
      .clk            (clk),
      .PCEnable       (enable),
      .areset         (areset),
      .address_to_load(address_to_load),
      .load_address   (load_address),
      .PC_Address     (PC_Address),
      .halt           (halt)
  );

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

  RegFile regfile (
      .clk           (clk),
      .RegWriteEnable(RegWrite),
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

  // ── PC Next ───────────────────────────────────────────────────
  wire [31:0] BranchAddr = PC_Address + imm_out;
  wire [31:0] JumpAddr = PC_Address + imm_out;

  // ── Branch Condition ──────────────────────────────────────────
  wire BranchTaken = ALUZero ^ imem_data[12];

  assign load_address = Jump | (Branch & BranchTaken);
  assign address_to_load = Jump ? JumpAddr : BranchAddr;

  // ── RegFile Write Data Mux ────────────────────────────────────
  assign WriteData = MemtoReg ? dmem_rdata : ALUOut;

  // ── Memory Interface ──────────────────────────────────────────
  assign imem_addr  = PC_Address >> 2;
  assign dmem_clk   = clk;
  assign dmem_we    = MemWrite;
  assign dmem_addr  = ALUOut;
  assign dmem_wdata = ReadRegOut2;

endmodule

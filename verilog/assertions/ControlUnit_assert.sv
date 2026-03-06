module ControlUnit_assert (
    input logic [31:0] instruction,
    input logic [ 1:0] ALUOps,
    input logic        ALUSrc,
    input logic        MemWrite,
    input logic        RegWrite,
    input logic        MemRead,
    input logic        MemtoReg,
    input logic        Branch,
    input logic        Jump
);

  wire [6:0] opcode = instruction[6:0];

  // 1. Identify the instruction types
  wire is_R = (opcode == 7'h33);
  wire is_I = (opcode == 7'h13);
  wire is_S = (opcode == 7'h23);
  wire is_B = (opcode == 7'h63);
  wire is_J = (opcode == 7'h6f);
  wire is_L = (opcode == 7'h03);

  // Group them into a 6-bit vector for structural checking
  wire [5:0] type_vector = {is_R, is_I, is_S, is_B, is_J, is_L};

  // 2. SVA Checks inside an always_comb block
  always_comb begin

    // --- STRUCTURAL CHECK ---
    // Ensures an instruction is never decoded as two different types at the same time.
    // The '1' in $fatal specifies the exit code for the simulator.
    assert_mutually_exclusive :
    assert ($onehot0(type_vector))
    else $fatal(1, "SVA FATAL: Instruction decoded as multiple types! Opcode: %h", opcode);

    // --- SIGNAL CHECKS ---
    if (is_R) begin
      assert_R_type :
      assert (RegWrite == 1 && MemWrite == 0 && MemRead == 0 && ALUSrc == 0 && ALUOps == 2'b10)
      else $error("SVA FAIL: R-Type signals incorrect!");
    end

    if (is_I) begin
      assert_I_type :
      assert (RegWrite == 1 && MemWrite == 0 && MemRead == 0 && ALUSrc == 1 && ALUOps == 2'b00)
      else $error("SVA FAIL: I-Type signals incorrect!");
    end

    if (is_S) begin
      assert_S_type :
      assert (RegWrite == 0 && MemWrite == 1 && MemRead == 0 && ALUSrc == 1 && ALUOps == 2'b00)
      else $error("SVA FAIL: S-Type signals incorrect!");
    end

    if (is_B) begin
      assert_B_type :
      assert (RegWrite == 0 && MemWrite == 0 && MemRead == 0 && Branch == 1 && ALUOps == 2'b01)
      else $error("SVA FAIL: B-Type signals incorrect!");
    end

    if (is_J) begin
      assert_J_type :
      assert (RegWrite == 1 && Jump == 1 && ALUSrc == 1)
      else $error("SVA FAIL: J-Type signals incorrect!");
    end

    if (is_L) begin
      assert_L_type :
      assert (RegWrite == 1 && MemWrite == 0 && MemRead == 1 && MemtoReg == 1 && ALUSrc == 1)
      else $error("SVA FAIL: Load-Type signals incorrect!");
    end

  end

endmodule

// --- BIND DIRECTIVE ---
// This tells the simulator to inject 'ControlUnit_assert' inside 'ControlUnit'
// automatically without modifying the actual RTL file.
bind ControlUnit ControlUnit_assert sva_checker (
    .instruction(instruction),
    .ALUOps(ALUOps),
    .ALUSrc(ALUSrc),
    .MemWrite(MemWrite),
    .RegWrite(RegWrite),
    .MemRead(MemRead),
    .MemtoReg(MemtoReg),
    .Branch(Branch),
    .Jump(Jump)
);

import alucu_pkg::*;
import alu_pkg::*;

module ALUControlUnit (
    input  logic    [1:0] ALUOp,
    input  logic    [2:0] funct3,
    input  logic          funct7_bit5,
    output alu_op_t       ALUCU_OUT
);

  logic use_f7;
  // funct7[5] only matters if funct3 is 000 (ADD/SUB) or 101 (SRL/SRA)
  assign use_f7 = (funct3 == 3'b000) || (funct3 == 3'b101);

  always_comb begin
    case (ALUOp)
      ALUOP_ADD: ALUCU_OUT = ALU_ADD;
      ALUOP_SUB: ALUCU_OUT = ALU_SUB;
      ALUOP_FUNC: ALUCU_OUT = alu_op_t'({(funct7_bit5 && use_f7), funct3});
      default: ALUCU_OUT = ALU_ADD;
    endcase
  end
endmodule

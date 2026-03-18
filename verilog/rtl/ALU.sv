/*
* An ALU should only do its ALU thingy.
* Therefore, it should only care about what operation it is doing, and its
* inputs which are x and y, and it only needs to resolve 8 operations:
* add, sub, xor, or, and, sll, srl, sra, slt, sltu
* Use 4 bit because in the future might add more opeartions, and it's just easier to not put the logic of SLT and SLTU in somewhere else. Although 3 bit is more efficient mux generate by the System verilog but yeah I am not sure how to handle SLT and SLTU without having additional signal that's wire to other module to wire to the register file
*/

import alu_pkg::*;

module ALU (
    input logic [31:0] x,
    input logic [31:0] y,
    input alu_op_t ALUops,
    output logic ALUZero,
    output logic ALUNegative,
    output logic [31:0] ALUOut
);


  // Using a share adder to calculate add, sub, slt, sltu

  logic [31:0] result;
  logic carry_out, negative, overflow, is_sub;
  logic [31:0] y_prime;

  assign is_sub = (ALUops == ALU_SUB) || (ALUops == ALU_SLT) || (ALUops == ALU_SLTU);
  assign y_prime = is_sub ? ~y : y;

  assign {carry_out, result} = {1'b0, x} + {1'b0, y_prime} + {32'b0, is_sub};
  assign negative = result[31];
  assign overflow = ~(x[31] ^ y_prime[31]) & (x[31] ^ result[31]);

  always_comb begin
    unique case (ALUops)
      ALU_ADD:  ALUOut = result;
      ALU_SUB:  ALUOut = result;
      ALU_XOR:  ALUOut = x ^ y;
      ALU_OR:   ALUOut = x | y;
      ALU_AND:  ALUOut = x & y;
      ALU_SLL:  ALUOut = x << y[4:0];
      ALU_SRL:  ALUOut = x >> y[4:0];
      ALU_SRA:  ALUOut = $signed(x) >>> y[4:0];
      ALU_SLT:  ALUOut = {31'b0, negative ^ overflow};
      ALU_SLTU: ALUOut = {31'b0, ~carry_out};
      default:  ALUOut = '0;
    endcase
  end

  assign ALUNegative = ALUOut[31];
  assign ALUZero     = (ALUOut == 32'b0);

endmodule

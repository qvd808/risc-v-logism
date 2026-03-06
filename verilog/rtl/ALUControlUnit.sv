module ALUControlUnit (
    input logic [1:0] ALUOp,
    input logic [2:0] funct3,
    input logic funct7_bit5,
    output logic [3:0] ALUCU_OUT
);

  always_comb begin
    casez ({
      ALUOp, funct3, funct7_bit5
    })
      6'b00_???_?: ALUCU_OUT = 4'b0010;  // Add

      6'b01_???_?: ALUCU_OUT = 4'b0110;  // Sub

      // ALUOp = 10 (R-Type / I-Type ALU operations)
      6'b10_000_0: ALUCU_OUT = 4'b0010;  // ADD
      6'b10_000_1: ALUCU_OUT = 4'b0110;  // SUB
      6'b10_100_?: ALUCU_OUT = 4'b0011;  // XOR
      6'b10_110_?: ALUCU_OUT = 4'b0001;  // OR
      6'b10_111_?: ALUCU_OUT = 4'b0000;  // AND

      // Default catches all your "----" don't care conditions safely
      default: ALUCU_OUT = 4'b0000;

    endcase
  end


endmodule

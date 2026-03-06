module ALU (
    input  [31:0] x,
    input  [31:0] y,
    input  [ 3:0] ALUops,
    output        ALUZero,
    output [31:0] ALUOut
);

  reg [31:0] result;

  always @(*) begin
    case (ALUops)
      4'b0000: result = x & y;  // AND
      4'b0001: result = x | y;  // OR
      4'b0010: result = x + y;  // ADD
      4'b0011: result = x ^ y;  // XOR
      4'b0110: result = x - y;  // SUB
      default: result = 32'b0;  // default
    endcase
  end

  assign ALUOut  = result;
  assign ALUZero = (ALUOut == 32'b0);

endmodule

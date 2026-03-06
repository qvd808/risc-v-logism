module ControlUnit (
    input [31:0] instruction,
    output [1:0] ALUOps,
    output ALUSrc,
    output MemWrite,
    output RegWrite,
    output MemRead,
    output MemtoReg,
    output Branch,
    output Jump
);

  wire [6:0] opcode = instruction[6:0];

  localparam logic [6:0] Rtype = 7'h33;
  localparam logic [6:0] Itype = 7'h13;
  localparam logic [6:0] Stype = 7'h23;
  localparam logic [6:0] Btype = 7'h63;
  localparam logic [6:0] Jtype = 7'h6f;
  localparam logic [6:0] LoadType = 7'h03;

  assign MemWrite = (opcode == Stype);

  assign RegWrite = (opcode == Rtype) | (opcode == Itype) |
                    (opcode == LoadType) | (opcode == Jtype);

  assign MemRead = (opcode == LoadType);
  assign MemtoReg = (opcode == LoadType);

  assign Branch = (opcode == Btype);
  assign Jump = (opcode == Jtype);

  /*
  * These type of instruction will need to compute the value from the
  * immediate
  */
  assign ALUSrc = (opcode == Stype) | (opcode == Itype) | (opcode == LoadType) | (opcode == Jtype);

  /*
   Rtype    → 10
   Btype    → 01
   Jtype    → 11 (don't care)
   Itype    → 00 (addi uses funct3 but ALUOp=00 still works via ALUCU)
   LoadType → 00 (always add for address)
   Stype    → 00 (always add for address)
  */
  assign ALUOps = (opcode == Rtype) ? 2'b10 : (opcode == Btype) ? 2'b01 : 2'b00;


endmodule

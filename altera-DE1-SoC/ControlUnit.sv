module ControlUnit (
    input [31:0] instruction,
    output reg [1:0] ALUOps,
    output reg ALUSrc,
    output reg MemWrite,
    output reg RegWrite,
    output reg MemRead,
    output reg MemtoReg,
    output reg Branch,
    output reg Jump,
    output wire halt_detected
);

  wire [6:0] opcode = instruction[6:0];

  // Instruction Type Constants
  localparam logic [6:0] Rtype = 7'h33;
  localparam logic [6:0] Itype = 7'h13;
  localparam logic [6:0] Stype = 7'h23;
  localparam logic [6:0] Btype = 7'h63;
  localparam logic [6:0] Jtype = 7'h6f;
  localparam logic [6:0] LoadType = 7'h03;

  // Combinational halt detection
  assign halt_detected = (instruction == 32'b0);

  always_comb begin
    ALUOps   = 2'b00;
    ALUSrc   = 0;
    MemWrite = 0;
    RegWrite = 0;
    MemRead  = 0;
    MemtoReg = 0;
    Branch   = 0;
    Jump     = 0;

    if (!halt_detected) begin
      case (opcode)
        Rtype: begin
          RegWrite = 1;
          ALUOps   = 2'b10;
        end
        Itype: begin
          RegWrite = 1;
          ALUSrc   = 1;
          ALUOps   = 2'b00;
        end
        LoadType: begin
          RegWrite = 1;
          ALUSrc   = 1;
          MemRead  = 1;
          MemtoReg = 1;
        end
        Stype: begin
          ALUSrc   = 1;
          MemWrite = 1;
        end
        Btype: begin
          Branch = 1;
          ALUOps = 2'b01;
        end
        Jtype: begin
          RegWrite = 1;
          Jump     = 1;
          ALUSrc   = 1;
        end
        default: ;
      endcase
    end
  end

endmodule

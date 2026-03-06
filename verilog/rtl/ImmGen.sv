module ImmGen (
    input [31:0] instruction,
    input Branch,
    input Jump,
    input MemWrite,
    output logic [31:0] imm_out
);


  wire [11:0] Iimm = instruction[31:20];
  wire [11:0] Simm = {{instruction[31:25]}, {instruction[11:7]}};
  wire [12:0] Bimm = {
    instruction[31], instruction[7], {instruction[30:25]}, {instruction[11:8]}, 1'b0
  };
  wire [20:0] Jimm = {
    instruction[31], {instruction[19:12]}, instruction[20], {instruction[30:21]}, 1'b0
  };

  wire ImmSel1 = Jump | MemWrite;
  wire ImmSel2 = Branch | Jump;

  always_comb begin
    imm_out = 32'b0;
    case ({
      ImmSel2, ImmSel1
    })
      2'b00: imm_out = {{20{Iimm[11]}}, Iimm};
      2'b01: imm_out = {{20{Simm[11]}}, Simm};
      2'b10: imm_out = {{19{Bimm[12]}}, Bimm};
      2'b11: imm_out = {{11{Jimm[20]}}, Jimm};
    endcase
  end

endmodule

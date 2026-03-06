module RegFile (
    input  logic        clk,
    input  logic        RegWriteEnable,
    input  logic        areset,
    input  logic [31:0] DATA,
    input  logic [ 4:0] ReadReg1Addr,
    input  logic [ 4:0] ReadReg2Addr,
    input  logic [ 4:0] WriteAddr,
    output logic [31:0] ReadRegOut1,
    output logic [31:0] ReadRegOut2
);

  logic [31:0] regs[32];

  always @(posedge clk or posedge areset) begin
    if (areset) begin
      for (int i = 0; i < 32; i++) regs[i] <= 32'b0;
    end else if (RegWriteEnable && WriteAddr != 0) begin
      regs[WriteAddr] <= DATA;
    end
  end

  always_comb begin
    ReadRegOut1 = (ReadReg1Addr == 0) ? 32'b0 : regs[ReadReg1Addr];
    ReadRegOut2 = (ReadReg2Addr == 0) ? 32'b0 : regs[ReadReg2Addr];
  end

endmodule

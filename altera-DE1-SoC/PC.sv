module PC (
    input clk,
    input PCEnable,
    input areset,
    input [31:0] address_to_load,
    input load_address,
    input halt,
    output [31:0] PC_Address
);

  reg [31:0] addr;
  always @(posedge clk, posedge areset) begin
    if (areset) addr <= 32'b0;
    else if (PCEnable && !halt) begin
      addr <= load_address ? address_to_load : addr + 4;
    end else addr <= addr;
  end

  assign PC_Address = addr;

endmodule

module PC_assert (
    input logic        clk,
    input logic        PCEnable,
    input logic        areset,
    input logic [31:0] address_to_load,
    input logic        load_address,
    input logic [31:0] PC_Address
);

  // ── Error flags (visible in waveform) ─────────────────────────
  logic sig_pc4_err = 0;
  logic sig_load_err = 0;
  logic sig_any_err = 0;

  initial begin
    $display("PC_assert module loaded successfully!");
  end

  // ── PC+4 every cycle ──────────────────────────────────────────
  Pc4_check :
  assert property (@(posedge clk) (!areset && PCEnable && !load_address) |=> (PC_Address == $past(
      PC_Address
  ) + 4))
  else begin
    sig_pc4_err = 1;
    sig_any_err = 1;
    $display("[FAIL] PC+4: got=0x%08h expected=0x%08h at time=%0t", PC_Address, $past(PC_Address)
             + 4, $time);
  end

  // ── Load address takes effect next cycle ──────────────────────
  // latch that areset fired between posedges
  logic areset_prev = 0;

  always @(negedge clk) begin  // captures areset on falling edge → visible at next posedge
    areset_prev <= areset;
  end

  Load_check :
  assert property (
    @(posedge clk)
    disable iff (areset || areset_prev)  // ← cancels promise if areset fired anywhere
  (PCEnable && load_address) |=> (PC_Address == $past(
      address_to_load
  )))
  else begin
    sig_load_err = 1;
    sig_any_err  = 1;
    $display("[FAIL] Load: got=0x%08h expected=0x%08h at time=%0t", PC_Address,
             $past(address_to_load), $time);
  end

endmodule

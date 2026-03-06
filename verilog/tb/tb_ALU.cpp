#include "VALU.h"
#include "tb_utils.h"

#define CLK_PERIOD 10

int main(int argc, char **argv) {
  // 1-line setup initializes Verilator, creates the DUT, and opens the VCD
  TestBench<VALU> tb(argc, argv, "sim/alu_waveform.vcd");

  std::cout << "=== ALU Tests ===" << std::endl;

  // ── AND ──────────────────────────────
  tb.dut->x = 0xFF00FF00;
  tb.dut->y = 0x0F0F0F0F;
  tb.dut->ALUops = 0b0000;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("AND ", tb.dut->ALUOut, 0x0F000F00);

  // ── OR ───────────────────────────────
  tb.dut->x = 0xFF00FF00;
  tb.dut->y = 0x0F0F0F0F;
  tb.dut->ALUops = 0b0001;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("OR  ", tb.dut->ALUOut, 0xFF0FFF0F);

  // ── ADD ──────────────────────────────
  tb.dut->x = 10;
  tb.dut->y = 20;
  tb.dut->ALUops = 0b0010;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("ADD ", tb.dut->ALUOut, 30);

  // ── XOR ──────────────────────────────
  tb.dut->x = 0xAAAAAAAA;
  tb.dut->y = 0x55555555;
  tb.dut->ALUops = 0b0011;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("XOR ", tb.dut->ALUOut, 0xFFFFFFFF);

  // ── SUB ──────────────────────────────
  tb.dut->x = 30;
  tb.dut->y = 10;
  tb.dut->ALUops = 0b0110;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SUB ", tb.dut->ALUOut, 20);

  // ── ALUZero (equal) ──────────────────
  tb.dut->x = 10;
  tb.dut->y = 10;
  tb.dut->ALUops = 0b0110;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check("ZERO", tb.dut->ALUZero, 1); // Standard check for 1-bit signal

  // ── Negative SUB ─────────────────────
  tb.dut->x = 5;
  tb.dut->y = 10;
  tb.dut->ALUops = 0b0110;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("NEG ", tb.dut->ALUOut, 0xFFFFFFFB);

  tb.show_summary();

  // Clean CI/CD exit: returns 0 if all passed, or >0 if there were failures
  return tb.fail_count;
}

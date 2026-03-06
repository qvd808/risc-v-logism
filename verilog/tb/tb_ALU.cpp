#include "VALU.h"
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define CLK_PERIOD 10 // 10 time units per test

// helper to print pass/fail
void check(const char *test, uint32_t got, uint32_t expected) {
  if (got == expected)
    std::cout << "PASS " << test << ": got 0x" << std::hex << got << std::endl;
  else
    std::cout << "FAIL " << test << ": got 0x" << std::hex << got
              << " expected 0x" << expected << std::endl;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  VALU *dut = new VALU;

  // --- Trace Setup ---
  Verilated::traceEverOn(true);
  VerilatedVcdC *tfp = new VerilatedVcdC;
  dut->trace(tfp, 99);
  tfp->open("sim/alu_waveform.vcd");

  int t = 0; // simulation time

  std::cout << "=== ALU Tests ===" << std::endl;

  // ── AND ──────────────────────────────
  dut->x = 0xFF00FF00;
  dut->y = 0x0F0F0F0F;
  dut->ALUops = 0b0000;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("AND ", dut->ALUOut, 0x0F000F00);

  // ── OR ───────────────────────────────
  dut->x = 0xFF00FF00;
  dut->y = 0x0F0F0F0F;
  dut->ALUops = 0b0001;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("OR  ", dut->ALUOut, 0xFF0FFF0F);

  // ── ADD ──────────────────────────────
  dut->x = 10;
  dut->y = 20;
  dut->ALUops = 0b0010;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("ADD ", dut->ALUOut, 30);

  // ── XOR ──────────────────────────────
  dut->x = 0xAAAAAAAA;
  dut->y = 0x55555555;
  dut->ALUops = 0b0011;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("XOR ", dut->ALUOut, 0xFFFFFFFF);

  // ── SUB ──────────────────────────────
  dut->x = 30;
  dut->y = 10;
  dut->ALUops = 0b0110;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("SUB ", dut->ALUOut, 20);

  // ── ALUZero (equal) ───────────────────
  dut->x = 10;
  dut->y = 10;
  dut->ALUops = 0b0110;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("ZERO", dut->ALUZero, 1);

  // ── Negative SUB ─────────────────────
  dut->x = 5;
  dut->y = 10;
  dut->ALUops = 0b0110;
  dut->eval();
  tfp->dump(t);
  t += CLK_PERIOD;
  check("NEG ", dut->ALUOut, 0xFFFFFFFB);

  std::cout << "=== Done ===" << std::endl;

  tfp->close();
  delete dut;
  return 0;
}

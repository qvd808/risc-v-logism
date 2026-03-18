#include "VALU.h"
#include "VALU_alu_pkg.h"
#include "tb_utils.h"

#define CLK_PERIOD 10

int main(int argc, char **argv) {
  TestBench<VALU> tb(argc, argv, "sim/alu_waveform.vcd");

  std::cout << "=== ALU Tests ===" << std::endl;

  // ── ADD ──────────────────────────────
  std::cout << "\n-- ADD --" << std::endl;

  // basic addition
  tb.dut->ALUops = VALU_alu_pkg::ALU_ADD;
  tb.dut->x = 10;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("ADD 10+5       ", tb.dut->ALUOut, 15);
  tb.check_signed("  isZero       ", tb.dut->ALUZero, 0);
  tb.check_signed("  isNegative   ", tb.dut->ALUNegative, 0);

  // zero result
  tb.dut->x = 0;
  tb.dut->y = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("ADD 0+0        ", tb.dut->ALUOut, 0);
  tb.check_signed("  isZero       ", tb.dut->ALUZero, 1);

  // overflow wraps (unsigned wrap)
  tb.dut->x = 0xFFFFFFFF;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("ADD overflow   ", tb.dut->ALUOut, 0x00000000);
  tb.check_signed("  isZero       ", tb.dut->ALUZero, 1);

  // negative result
  tb.dut->x = -32;
  tb.dut->y = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("ADD -32+0   ", tb.dut->ALUOut, -32);
  tb.check_signed("  isNegative   ", tb.dut->ALUNegative, 1);

  // ── SUB ──────────────────────────────
  std::cout << "\n-- SUB --" << std::endl;

  // basic subtraction
  tb.dut->ALUops = VALU_alu_pkg::ALU_SUB;
  tb.dut->x = 10;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("SUB 10-5       ", tb.dut->ALUOut, 5);
  tb.check_signed("  isZero       ", tb.dut->ALUZero, 0);
  tb.check_signed("  isNegative   ", tb.dut->ALUNegative, 0);

  // zero result
  tb.dut->x = 5;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("SUB 5-5        ", tb.dut->ALUOut, 0);
  tb.check_signed("  isZero       ", tb.dut->ALUZero, 1);

  // negative result
  tb.dut->x = 3;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("SUB 3-5        ", tb.dut->ALUOut, 0xFFFFFFFE);
  tb.check_signed("  isNegative   ", tb.dut->ALUNegative, 1);

  tb.dut->x = -3;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_signed("SUB -3-5        ", tb.dut->ALUOut, 0xFFFFFFF8);
  tb.check_signed("  isNegative   ", tb.dut->ALUNegative, 1);

  // ── XOR ──────────────────────────────
  std::cout << "\n-- XOR --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_XOR;
  tb.dut->x = 0xFF00FF00;
  tb.dut->y = 0x0F0F0F0F;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("XOR pattern    ", tb.dut->ALUOut, 0xF00FF00F);
  tb.check_hex("  isZero       ", tb.dut->ALUZero, 0);

  // XOR with self = 0
  tb.dut->x = 0xDEADBEEF;
  tb.dut->y = 0xDEADBEEF;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("XOR self       ", tb.dut->ALUOut, 0);
  tb.check_hex("  isZero       ", tb.dut->ALUZero, 1);

  // ── OR ───────────────────────────────
  std::cout << "\n-- OR --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_OR;
  tb.dut->x = 0xFF00FF00;
  tb.dut->y = 0x0F0F0F0F;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("OR  pattern    ", tb.dut->ALUOut, 0xFF0FFF0F);
  tb.check_hex("  isZero       ", tb.dut->ALUZero, 0);

  // OR with 0 = identity
  tb.dut->x = 0xDEADBEEF;
  tb.dut->y = 0x00000000;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("OR  identity   ", tb.dut->ALUOut, 0xDEADBEEF);

  // ── AND ──────────────────────────────
  std::cout << "\n-- AND --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_AND;
  tb.dut->x = 0xFF00FF00;
  tb.dut->y = 0x0F0F0F0F;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("AND pattern    ", tb.dut->ALUOut, 0x0F000F00);
  tb.check_hex("  isZero       ", tb.dut->ALUZero, 0);

  // AND with 0 = 0
  tb.dut->x = 0xDEADBEEF;
  tb.dut->y = 0x00000000;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("AND zero       ", tb.dut->ALUOut, 0);
  tb.check_hex("  isZero       ", tb.dut->ALUZero, 1);

  // AND with all ones = identity
  tb.dut->x = 0xDEADBEEF;
  tb.dut->y = 0xFFFFFFFF;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("AND identity   ", tb.dut->ALUOut, 0xDEADBEEF);

  // ── SLL ──────────────────────────────
  std::cout << "\n-- SLL --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_SLL;

  // shift by 1
  tb.dut->x = 0x00000001;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLL 1<<1       ", tb.dut->ALUOut, 0x00000002);

  // shift by 4
  tb.dut->x = 0x0000000F;
  tb.dut->y = 4;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLL 0xF<<4     ", tb.dut->ALUOut, 0x000000F0);

  // shift out all bits
  tb.dut->x = 0x00000001;
  tb.dut->y = 31;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLL 1<<31      ", tb.dut->ALUOut, 0x80000000);
  tb.check_hex("  isNegative   ", tb.dut->ALUNegative, 1);

  // only lower 5 bits of y used
  tb.dut->x = 0x00000001;
  tb.dut->y = 32; // y[4:0] = 0, so shift by 0
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLL y[4:0]=0   ", tb.dut->ALUOut, 0x00000001);

  // ── SRL ──────────────────────────────
  std::cout << "\n-- SRL --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_SRL;

  // basic shift
  tb.dut->x = 0x00000008;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SRL 8>>1       ", tb.dut->ALUOut, 0x00000004);

  // MSB should NOT be sign extended (logical)
  tb.dut->x = 0x80000000;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SRL logical    ", tb.dut->ALUOut, 0x40000000);
  tb.check_hex("  isNegative   ", tb.dut->ALUNegative, 0);

  // shift by 31
  tb.dut->x = 0x80000000;
  tb.dut->y = 31;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SRL >>31       ", tb.dut->ALUOut, 0x00000001);

  // ── SRA ──────────────────────────────
  std::cout << "\n-- SRA --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_SRA;

  // positive number: same as SRL
  tb.dut->x = 0x00000008;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SRA +8>>1      ", tb.dut->ALUOut, 0x00000004);

  // negative number: MSB sign extended
  tb.dut->x = 0x80000000;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SRA -MSB>>1    ", tb.dut->ALUOut, 0xC0000000);
  tb.check_hex("  isNegative   ", tb.dut->ALUNegative, 1);

  // shift -1 by any amount stays -1
  tb.dut->x = 0xFFFFFFFF;
  tb.dut->y = 31;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SRA -1>>31     ", tb.dut->ALUOut, 0xFFFFFFFF);
  tb.check_hex("  isNegative   ", tb.dut->ALUNegative, 1);

  // ── SLT ──────────────────────────────
  std::cout << "\n-- SLT (signed) --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_SLT;

  // 1 < 2 → 1
  tb.dut->x = 1;
  tb.dut->y = 2;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT 1<2        ", tb.dut->ALUOut, 1);

  // 2 < 1 → 0
  tb.dut->x = 2;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT 2<1        ", tb.dut->ALUOut, 0);

  // equal → 0
  tb.dut->x = 5;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT 5<5        ", tb.dut->ALUOut, 0);

  // -1 < 1 (signed) → 1
  tb.dut->x = 0xFFFFFFFF; // -1
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT -1<1       ", tb.dut->ALUOut, 1);

  // 1 < -1 (signed) → 0
  tb.dut->x = 1;
  tb.dut->y = 0xFFFFFFFF; // -1
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT 1<-1       ", tb.dut->ALUOut, 0);

  // overflow edge case: INT_MAX < INT_MIN (signed) → 0
  tb.dut->x = 0x7FFFFFFF; // INT_MAX
  tb.dut->y = 0x80000000; // INT_MIN
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT MAX<MIN    ", tb.dut->ALUOut, 0);

  // INT_MIN < INT_MAX (signed) → 1
  tb.dut->x = 0x80000000; // INT_MIN
  tb.dut->y = 0x7FFFFFFF; // INT_MAX
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLT MIN<MAX    ", tb.dut->ALUOut, 1);

  // ── SLTU ─────────────────────────────
  std::cout << "\n-- SLTU (unsigned) --" << std::endl;

  tb.dut->ALUops = VALU_alu_pkg::ALU_SLTU;

  // 1 < 2 → 1
  tb.dut->x = 1;
  tb.dut->y = 2;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLTU 1<2       ", tb.dut->ALUOut, 1);

  // 2 < 1 → 0
  tb.dut->x = 2;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLTU 2<1       ", tb.dut->ALUOut, 0);

  // equal → 0
  tb.dut->x = 5;
  tb.dut->y = 5;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLTU 5<5       ", tb.dut->ALUOut, 0);

  // 0xFFFFFFFF is large unsigned, NOT less than 1
  tb.dut->x = 0xFFFFFFFF;
  tb.dut->y = 1;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLTU 0xFF...<1 ", tb.dut->ALUOut, 0);

  // 1 < 0xFFFFFFFF (unsigned) → 1
  tb.dut->x = 1;
  tb.dut->y = 0xFFFFFFFF;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLTU 1<0xFF... ", tb.dut->ALUOut, 1);

  // 0 < anything nonzero → 1
  tb.dut->x = 0;
  tb.dut->y = 0x80000000;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("SLTU 0<0x80... ", tb.dut->ALUOut, 1);

  tb.show_summary();
  return tb.fail_count;
}

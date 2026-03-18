#include "VALUControlUnit.h"
#include "VALUControlUnit_alu_pkg.h"
#include "VALUControlUnit_alucu_pkg.h"
#include "tb_utils.h"

using alu = VALUControlUnit_alu_pkg;
using alucu = VALUControlUnit_alucu_pkg;

int main(int argc, char **argv) {
  TestBench<VALUControlUnit> tb(argc, argv, "sim/alucu_waveform.vcd");

  std::cout << "=== ALU Control Unit Tests ===" << std::endl;

  // helper lambda to reduce boilerplate
  auto test = [&](const char *name, uint8_t aluop, uint8_t funct3, uint8_t f7b5,
                  uint8_t expected) {
    tb.dut->ALUOp = aluop;
    tb.dut->funct3 = funct3;
    tb.dut->funct7_bit5 = f7b5;
    tb.dut->eval();
    tb.check_bin(name, tb.dut->ALUCU_OUT, expected);
  };

  // ── ALUOp = 00 (always ADD, ignore funct3/funct7) ────────────────────────
  std::cout << "\n-- ALUOp=00: forced ADD (Load/Store/JALR) --" << std::endl;

  // funct3 and funct7 should be completely ignored
  test("00 f3=000 f7=0 → ADD", alucu::ALUOP_ADD, 0b000, 0, alu::ALU_ADD);
  test("00 f3=000 f7=1 → ADD", alucu::ALUOP_ADD, 0b000, 1, alu::ALU_ADD);
  test("00 f3=101 f7=1 → ADD", alucu::ALUOP_ADD, 0b101, 1, alu::ALU_ADD);
  test("00 f3=111 f7=1 → ADD", alucu::ALUOP_ADD, 0b111, 1, alu::ALU_ADD);

  // ── ALUOp = 01 (always SUB, ignore funct3/funct7) ────────────────────────
  std::cout << "\n-- ALUOp=01: forced SUB (Branch) --" << std::endl;

  test("01 f3=000 f7=0 → SUB", alucu::ALUOP_SUB, 0b000, 0, alu::ALU_SUB);
  test("01 f3=000 f7=1 → SUB", alucu::ALUOP_SUB, 0b000, 1, alu::ALU_SUB);
  test("01 f3=101 f7=1 → SUB", alucu::ALUOP_SUB, 0b101, 1, alu::ALU_SUB);
  test("01 f3=111 f7=0 → SUB", alucu::ALUOP_SUB, 0b111, 0, alu::ALU_SUB);

  // ── ALUOp = 10, funct3=000 (ADD vs SUB) ──────────────────────────────────
  // funct7[5] IS used for funct3=000
  std::cout << "\n-- ALUOp=10 funct3=000: ADD vs SUB --" << std::endl;

  test("10 f3=000 f7=0 → ADD", alucu::ALUOP_FUNC, 0b000, 0, alu::ALU_ADD);
  test("10 f3=000 f7=1 → SUB", alucu::ALUOP_FUNC, 0b000, 1, alu::ALU_SUB);

  // ── ALUOp = 10, funct3=001 (SLL) ─────────────────────────────────────────
  // funct7[5] is NOT used for funct3=001 — must always be SLL
  std::cout << "\n-- ALUOp=10 funct3=001: SLL (f7 ignored) --" << std::endl;

  test("10 f3=001 f7=0 → SLL", alucu::ALUOP_FUNC, 0b001, 0, alu::ALU_SLL);
  test("10 f3=001 f7=1 → SLL", alucu::ALUOP_FUNC, 0b001, 1,
       alu::ALU_SLL); // f7 ignored

  // ── ALUOp = 10, funct3=010 (SLT) ─────────────────────────────────────────
  std::cout << "\n-- ALUOp=10 funct3=010: SLT (f7 ignored) --" << std::endl;

  test("10 f3=010 f7=0 → SLT ", alucu::ALUOP_FUNC, 0b010, 0, alu::ALU_SLT);
  test("10 f3=010 f7=1 → SLT ", alucu::ALUOP_FUNC, 0b010, 1,
       alu::ALU_SLT); // f7 ignored

  // ── ALUOp = 10, funct3=011 (SLTU) ────────────────────────────────────────
  std::cout << "\n-- ALUOp=10 funct3=011: SLTU (f7 ignored) --" << std::endl;

  test("10 f3=011 f7=0 → SLTU", alucu::ALUOP_FUNC, 0b011, 0, alu::ALU_SLTU);
  test("10 f3=011 f7=1 → SLTU", alucu::ALUOP_FUNC, 0b011, 1,
       alu::ALU_SLTU); // f7 ignored

  // ── ALUOp = 10, funct3=100 (XOR) ─────────────────────────────────────────
  std::cout << "\n-- ALUOp=10 funct3=100: XOR (f7 ignored) --" << std::endl;

  test("10 f3=100 f7=0 → XOR ", alucu::ALUOP_FUNC, 0b100, 0, alu::ALU_XOR);
  test("10 f3=100 f7=1 → XOR ", alucu::ALUOP_FUNC, 0b100, 1,
       alu::ALU_XOR); // f7 ignored

  // ── ALUOp = 10, funct3=101 (SRL vs SRA) ──────────────────────────────────
  // funct7[5] IS used for funct3=101
  std::cout << "\n-- ALUOp=10 funct3=101: SRL vs SRA --" << std::endl;

  test("10 f3=101 f7=0 → SRL ", alucu::ALUOP_FUNC, 0b101, 0, alu::ALU_SRL);
  test("10 f3=101 f7=1 → SRA ", alucu::ALUOP_FUNC, 0b101, 1, alu::ALU_SRA);

  // ── ALUOp = 10, funct3=110 (OR) ──────────────────────────────────────────
  std::cout << "\n-- ALUOp=10 funct3=110: OR (f7 ignored) --" << std::endl;

  test("10 f3=110 f7=0 → OR  ", alucu::ALUOP_FUNC, 0b110, 0, alu::ALU_OR);
  test("10 f3=110 f7=1 → OR  ", alucu::ALUOP_FUNC, 0b110, 1,
       alu::ALU_OR); // f7 ignored

  // ── ALUOp = 10, funct3=111 (AND) ─────────────────────────────────────────
  std::cout << "\n-- ALUOp=10 funct3=111: AND (f7 ignored) --" << std::endl;

  test("10 f3=111 f7=0 → AND ", alucu::ALUOP_FUNC, 0b111, 0, alu::ALU_AND);
  test("10 f3=111 f7=1 → AND ", alucu::ALUOP_FUNC, 0b111, 1,
       alu::ALU_AND); // f7 ignored

  // ── ALUOp = 11 (default, should be ADD) ──────────────────────────────────
  std::cout << "\n-- ALUOp=11: default → ADD --" << std::endl;

  test("11 f3=101 f7=1 → ADD", alucu::ALUOP_RESERVE, 0b101, 1, alu::ALU_ADD);

  tb.show_summary();
  return tb.fail_count;
}

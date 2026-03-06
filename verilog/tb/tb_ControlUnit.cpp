#include "VControlUnit.h"
#include "tb_utils.h"

// ── drive instruction, dump waveform, check outputs ───────────────
void test_instruction(TestBench<VControlUnit> &tb, const char *name,
                      uint32_t instruction, uint8_t exp_RegWrite,
                      uint8_t exp_MemWrite, uint8_t exp_MemRead,
                      uint8_t exp_MemtoReg, uint8_t exp_ALUSrc,
                      uint8_t exp_Branch, uint8_t exp_Jump,
                      uint8_t exp_ALUOps) {

  // Using C++20 format for a much cleaner header!
  std::cout << std::format("\n=== {} ({:#010x}) ===\n", name, instruction);

  // ── drive input ───────────────────────────────────────────────
  tb.dut->instruction = instruction;
  tb.dut->eval();

  // ── dump BEFORE checks so waveform captures input state ───────
  tb.tfp->dump(tb.ctx->time());

  // ── C++ checks ────────────────────────────────────────────────
  int failures = 0;
  failures += tb.check("RegWrite", tb.dut->RegWrite, exp_RegWrite);
  failures += tb.check("MemWrite", tb.dut->MemWrite, exp_MemWrite);
  failures += tb.check("MemRead ", tb.dut->MemRead, exp_MemRead);
  failures += tb.check("MemtoReg", tb.dut->MemtoReg, exp_MemtoReg);
  failures += tb.check("ALUSrc  ", tb.dut->ALUSrc, exp_ALUSrc);
  failures += tb.check("Branch  ", tb.dut->Branch, exp_Branch);
  failures += tb.check("Jump    ", tb.dut->Jump, exp_Jump);
  failures += tb.check("ALUOps  ", tb.dut->ALUOps, exp_ALUOps);

  if (failures > 0) {
    // ── extra dump cycles so failing test is wider in GTKWave ─
    tb.tick(10);
    tb.dut->eval();
    tb.tfp->dump(tb.ctx->time());
    tb.tick(10);
    tb.dut->eval();
    tb.tfp->dump(tb.ctx->time());
    tb.tick(10);
    tb.dut->eval();
    tb.tfp->dump(tb.ctx->time());
  }

  // Increment time for the next instruction so they don't overlap
  tb.tick(10);
}

int main(int argc, char **argv) {
  TestBench<VControlUnit> tb(argc, argv, "sim/cu_waveform.vcd");

  std::cout << "=== ControlUnit Tests ===" << std::endl;

  //                name            instr      RW MW MR M2R AS BR JMP OP
  test_instruction(tb, "addi x1,x0,5", 0x00500093, 1, 0, 0, 0, 1, 0, 0, 0b00);
  test_instruction(tb, "add x1,x2,x3", 0x003100B3, 1, 0, 0, 0, 0, 0, 0, 0b10);
  test_instruction(tb, "sub x1,x2,x3", 0x403100B3, 1, 0, 0, 0, 0, 0, 0, 0b10);
  test_instruction(tb, "lw x1,0(x2)", 0x00012083, 1, 0, 1, 1, 1, 0, 0, 0b00);
  test_instruction(tb, "sw x1,0(x2)", 0x00112023, 0, 1, 0, 0, 1, 0, 0, 0b00);
  test_instruction(tb, "beq x1,x2,0", 0x00208063, 0, 0, 0, 0, 0, 1, 0, 0b01);
  test_instruction(tb, "bne x1,x2,0", 0x00209063, 0, 0, 0, 0, 0, 1, 0, 0b01);

  // JAL uses 0 for ALUOps as per your "keep it simple" decision
  test_instruction(tb, "jal x1,0", 0x000000EF, 1, 0, 0, 0, 1, 0, 1, 0b11);

  test_instruction(tb, "and x1,x2,x3", 0x0031F0B3, 1, 0, 0, 0, 0, 0, 0, 0b10);
  test_instruction(tb, "or x1,x2,x3", 0x0031E0B3, 1, 0, 0, 0, 0, 0, 0, 0b10);
  test_instruction(tb, "xor x1,x2,x3", 0x0031C0B3, 1, 0, 0, 0, 0, 0, 0, 0b10);

  tb.show_summary();

  return tb.fail_count;
}

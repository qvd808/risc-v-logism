#include "VImmGen.h"
#include "tb_utils.h"

#define CLK_PERIOD 10

int main(int argc, char **argv) {
  TestBench<VImmGen> tb(argc, argv, "sim/immgen_waveform.vcd");

  std::cout << "=== ImmGen Tests ===" << std::endl;

  tb.dut->instruction = 0x00500093;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("addi x1, x0, 5 ", tb.dut->imm_out, 0x00000005);

  tb.dut->instruction = 0x00802103;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("lw x2, 8(x0) ", tb.dut->imm_out, 0x00000008);

  tb.dut->instruction = 0x00202623;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 1;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("sw x2, 12(x0) ", tb.dut->imm_out, 0x0000000C);

  tb.dut->instruction = 0x00000863;
  tb.dut->Branch = 1;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("beq x0, x0, 16 ", tb.dut->imm_out, 0x00000010);

  tb.dut->instruction = 0x020000EF;
  tb.dut->Branch = 0;
  tb.dut->Jump = 1;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("jal x1, 32 ", tb.dut->imm_out, 0x00000020);

  tb.dut->instruction = 0xFFF00093;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("addi x1, x0, -1 ", tb.dut->imm_out, 0xFFFFFFFF);

  tb.dut->instruction = 0xFFC00103;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("lw x2, -4(x0) ", tb.dut->imm_out, 0xFFFFFFFC);

  tb.dut->instruction = 0xFE202C23;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 1;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("sw x2, -8(x0)  ", tb.dut->imm_out, 0xFFFFFFF8);

  tb.dut->instruction = 0xFE202623;
  tb.dut->Branch = 0;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 1;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("sw x2, -20(x0) ", tb.dut->imm_out, 0xFFFFFFEC);

  tb.dut->instruction = 0xFE000CE3;
  tb.dut->Branch = 1;
  tb.dut->Jump = 0;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("beq x0, x0, -8 ", tb.dut->imm_out, 0xFFFFFFF8);

  tb.dut->instruction = 0xFE1FF0EF;
  tb.dut->Branch = 0;
  tb.dut->Jump = 1;
  tb.dut->MemWrite = 0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check_hex("jal x1, -32 ", tb.dut->imm_out, 0xFFFFFFE0);

  tb.show_summary();

  return tb.fail_count;
}

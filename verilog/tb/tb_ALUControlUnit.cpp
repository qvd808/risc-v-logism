#include "VALUControlUnit.h"
#include "tb_utils.h"

#define CLK_PERIOD

int main(int argc, char **argv) {
  TestBench<VALUControlUnit> tb(argc, argv, "sim/alucu_waveform.vcd");

  std::cout << "=== ALU ControlUnit Tests ===" << std::endl;

  tb.dut->ALUOp = 0b00;
  tb.dut->funct3 = 0b100;    // random should not matter
  tb.dut->funct7_bit5 = 0b0; // random should not matter
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("ADD ", tb.dut->ALUCU_OUT, 0b0010);

  tb.dut->ALUOp = 0b01;
  tb.dut->funct3 = 0b001;    // random should not matter
  tb.dut->funct7_bit5 = 0b1; // random should not matter
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("SUB ", tb.dut->ALUCU_OUT, 0b0110);

  tb.dut->ALUOp = 0b10;
  tb.dut->funct3 = 0b000;
  tb.dut->funct7_bit5 = 0b0;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("ADD ", tb.dut->ALUCU_OUT, 0b0010);

  tb.dut->ALUOp = 0b10;
  tb.dut->funct3 = 0b000;
  tb.dut->funct7_bit5 = 0b1;
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("SUB ", tb.dut->ALUCU_OUT, 0b0110);

  tb.dut->ALUOp = 0b10;
  tb.dut->funct3 = 0b100;
  tb.dut->funct7_bit5 = 0b1; // random should not matter
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("XOR ", tb.dut->ALUCU_OUT, 0b0011);

  tb.dut->ALUOp = 0b10;
  tb.dut->funct3 = 0b110;
  tb.dut->funct7_bit5 = 0b1; // random should not matter
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("OR ", tb.dut->ALUCU_OUT, 0b0001);

  tb.dut->ALUOp = 0b10;
  tb.dut->funct3 = 0b111;
  tb.dut->funct7_bit5 = 0b1; // random should not matter
  tb.dut->eval();
  tb.tfp->dump(tb.ctx->time());
  tb.tick(CLK_PERIOD);
  tb.check("AND ", tb.dut->ALUCU_OUT, 0b0000);

  tb.show_summary();

  return tb.fail_count;
}

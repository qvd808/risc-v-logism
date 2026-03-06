#include "VRegFile.h"
#include "tb_utils.h"

#define CLK_PERIOD 10

int main(int argc, char **argv) {
  TestBench<VRegFile> tb(argc, argv, "sim/regfile_waveform.vcd");

  std::cout << "=== RegFile Tests ===" << std::endl;

  tb.dut->RegWriteEnable = 0;
  tb.dut->DATA = 0x8;
  tb.dut->ReadReg1Addr = 0b00000;
  tb.dut->ReadReg2Addr = 0b00001;
  tb.dut->WriteAddr = 1;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x0", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x1", tb.dut->ReadRegOut2, 0x00000000);

  tb.dut->RegWriteEnable = 0;
  tb.dut->DATA = 0x12;
  tb.dut->ReadReg1Addr = 0b00100;
  tb.dut->ReadReg2Addr = 0b10001;
  tb.dut->WriteAddr = 1;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x4", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x17", tb.dut->ReadRegOut2, 0x00000000);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x16;
  tb.dut->ReadReg1Addr = 0b00100;
  tb.dut->ReadReg2Addr = 0b10001;
  tb.dut->WriteAddr = 0b10001;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x4", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x17", tb.dut->ReadRegOut2, 0x00000016);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x8;
  tb.dut->ReadReg1Addr = 0b00100;
  tb.dut->ReadReg2Addr = 0b10001;
  tb.dut->WriteAddr = 0b00100;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x4", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x17", tb.dut->ReadRegOut2, 0x00000016);

  tb.dut->RegWriteEnable = 0;
  tb.dut->DATA = 0x8;
  tb.dut->ReadReg1Addr = 0b00100;
  tb.dut->ReadReg2Addr = 0b10001;
  tb.dut->WriteAddr = 0b00100;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x4", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x17", tb.dut->ReadRegOut2, 0x00000016);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00100;
  tb.dut->ReadReg2Addr = 0b10001;
  tb.dut->WriteAddr = 0b00100;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x4", tb.dut->ReadRegOut1, 0x00000021);
  tb.check_hex("ReadRegOut2 x17", tb.dut->ReadRegOut2, 0x00000016);

  tb.dut->RegWriteEnable = 0;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00000;
  tb.dut->ReadReg2Addr = 0b00001;
  tb.dut->WriteAddr = 0b00000;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x0", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x1", tb.dut->ReadRegOut2, 0x00000000);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00000;
  tb.dut->ReadReg2Addr = 0b00001;
  tb.dut->WriteAddr = 0b00000;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x0", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x1", tb.dut->ReadRegOut2, 0x00000000);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00000;
  tb.dut->ReadReg2Addr = 0b00001;
  tb.dut->WriteAddr = 0b00001;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x0", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x1", tb.dut->ReadRegOut2, 0x00000000);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00000;
  tb.dut->ReadReg2Addr = 0b00001;
  tb.dut->WriteAddr = 0b00001;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x0", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x1", tb.dut->ReadRegOut2, 0x00000021);

  tb.dut->RegWriteEnable = 1;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00000;
  tb.dut->ReadReg2Addr = 0b00001;
  tb.dut->WriteAddr = 0b00001;
  tb.dut->areset = 1;

  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x0", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x1", tb.dut->ReadRegOut2, 0x00000000);

  tb.dut->RegWriteEnable = 0;
  tb.dut->DATA = 0x21;
  tb.dut->ReadReg1Addr = 0b00100;
  tb.dut->ReadReg2Addr = 0b10001;
  tb.dut->WriteAddr = 0b00100;

  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("ReadRegOut1 x4", tb.dut->ReadRegOut1, 0x00000000);
  tb.check_hex("ReadRegOut2 x17", tb.dut->ReadRegOut2, 0x00000000);

  tb.half_tick(CLK_PERIOD);
  tb.half_tick(CLK_PERIOD);

  tb.show_summary();

  return tb.fail_count;
}

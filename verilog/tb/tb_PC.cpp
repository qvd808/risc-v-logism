#include "VPC.h"
#include "tb_utils.h"

#define CLK_PERIOD 10

int main(int argc, char **argv) {
  TestBench<VPC> tb(argc, argv, "sim/pc_waveform.vcd");

  std::cout << "=== PC Tests ===" << std::endl;

  tb.dut->PCEnable = 1;
  tb.dut->address_to_load = 0x8;
  tb.dut->load_address = 1;
  tb.dut->areset = 0;
  tb.dut->eval();
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("PC load on start", tb.dut->PC_Address, 0x00000008);

  tb.dut->PCEnable = 1;
  tb.dut->areset = 1;
  tb.dut->address_to_load = 0x12;
  tb.dut->load_address = 1;
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("Should not load on failing edge", tb.dut->PC_Address,
               0x00000000);

  tb.dut->PCEnable = 1;
  tb.dut->areset = 0;
  tb.dut->address_to_load = 0x12;
  tb.dut->load_address = 0;
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("PC + 4 ", tb.dut->PC_Address, 0x00000004);

  tb.dut->PCEnable = 1;
  tb.dut->areset = 0;
  tb.dut->address_to_load = 0x12;
  tb.dut->load_address = 0;
  tb.half_tick(CLK_PERIOD);
  tb.check_hex("PC + 4 failling edge should not update", tb.dut->PC_Address,
               0x00000004);

  tb.dut->PCEnable = 1;
  tb.dut->areset = 0;
  tb.dut->address_to_load = 0x12;
  tb.dut->load_address = 0;
  tb.tick(CLK_PERIOD);
  tb.check_hex("PC + 8 ", tb.dut->PC_Address, 0x00000008);

  tb.dut->PCEnable = 0;
  tb.dut->areset = 0;
  tb.dut->address_to_load = 0x12;
  tb.dut->load_address = 0;
  tb.tick(CLK_PERIOD);
  tb.check_hex("PC should not update due to not Enable", tb.dut->PC_Address,
               0x00000008);

  tb.dut->PCEnable = 1;
  tb.dut->areset = 0;
  tb.dut->address_to_load = 0x12;
  tb.dut->load_address = 1;
  tb.tick(CLK_PERIOD);
  tb.check_hex("PC jumps to 0x12", tb.dut->PC_Address, 0x00000012);

  tb.show_summary();

  return tb.fail_count;
}

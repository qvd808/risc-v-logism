#include "Vmain.h"
#include "tb_utils.h"

#define CLK_PERIOD 10

int main(int argc, char **argv) {
  TestBench<Vmain> tb(argc, argv, "sim/cpu_waveform.vcd");

  std::cout << "=== CPU Tests ===" << std::endl;

  tb.show_summary();

  return tb.fail_count;
}

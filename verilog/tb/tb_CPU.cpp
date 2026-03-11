#include "Vmain.h"
#include "Vmain___024root.h"
#include "tb_utils.h"
#include <cstdint>
#include <format>
#include <ostream>
#include <vector>

#define CLK_PERIOD 10

// ── Mock ROM ──────────────────────────────────────────────────────
static const uint32_t ROM[256] = {
    0x00000093, // 0:  addi x1, x0, 0
    0x00a00113, // 1:  addi x2, x0, 10
    0x002080b3, // 2:  add  x1, x1, x2
    0xfff10113, // 3:  addi x2, x2, -1
    0xfe011ce3, // 4:  bne  x2, x0, -8
    0x00000193, // 5:  addi x3, x0, 0
    0x0011a023, // 6:  sw   x1, 0(x3)
    0x0001a283, // 7:  lw   x5, 0(x3)
    0x0100006f, // 8:  jal  x0, 16
    0x00000013, // 9:  nop
    0x00000013, // 10: nop
    0x00000013, // 11: nop
    0x00528293  // 12: addi x5, x5, 5
};

// ── Synchronous RAM ───────────────────────────────────────────────
struct RAM {
  uint32_t mem[256] = {0};

  // Clocked on same rising edge as CPU
  void tick(TestBench<Vmain> &tb) {
    // Write on rising edge
    if (tb.dut->dmem_we) {
      uint32_t addr = tb.dut->dmem_addr >> 2;
      if (addr < 256)
        mem[addr] = tb.dut->dmem_wdata;
    }

    // Drive read data combinationally after edge and re-evaluate
    uint32_t addr = tb.dut->dmem_addr >> 2;
    tb.dut->dmem_rdata = (addr < 256) ? mem[addr] : 0;
    tb.dut->eval(); // propagate dmem_rdata through CPU combinational logic
  }

  uint32_t read(uint32_t word_addr) const { return mem[word_addr]; }
};

// ── CPU State Snapshot ────────────────────────────────────────────
struct CycleState {
  int cycle;
  uint32_t pc;
  uint32_t regs[11]; // x0-x10
  uint32_t ram_state;
};

// ── Feed ROM combinationally ───────────────────────────────────────
void rom_eval(TestBench<Vmain> &tb) {
  tb.dut->imem_data = ROM[tb.dut->imem_addr];
}

// ── One CPU cycle ──────────────────────────────────────────────────
void cpu_tick(TestBench<Vmain> &tb, RAM &ram) {
  rom_eval(tb);
  tb.tick(CLK_PERIOD);
  ram.tick(tb); // RAM ticked after clock edge, same as CPU
}

// // ── Snapshot current CPU state ────────────────────────────────────
// CycleState snapshot(TestBench<Vmain> &tb, int cycle) {
//   CycleState s;
//   s.cycle = cycle;
//   s.pc = tb.dut->rootp->main__DOT__pc__DOT__addr;
//   s.regs[0] = 0; // x0 hardwired
//   for (int i = 1; i <= 10; i++)
//     s.regs[i] = tb.dut->rootp->main__DOT__regfile__DOT__regs[i];
//   return s;
// }
//
// // ── Print table ───────────────────────────────────────────────────
// void print_table(const std::vector<CycleState> &table) {
//   // header
//   std::cout << std::format("{:>6} | {:>10} |", "cycle", "PC");
//   for (int i = 0; i <= 10; i++)
//     std::cout << std::format(" {:>10} |", std::format("x{}", i));
//   std::cout << "\n";
//
//   // separator
//   std::cout << std::string(6 + 3 + 12 + 11 * 13, '-') << "\n";
//
//   // rows
//   for (const auto &s : table) {
//     std::cout << std::format("{:>6} | {:#010x} |", s.cycle, s.pc);
//     for (int i = 0; i <= 10; i++)
//       std::cout << std::format(" {:#010x} |", s.regs[i]);
//     std::cout << "\n";
//   }
// }

// ── Expected Results ──────────────────────────────────────────────
static CycleState c[] = {
    {.cycle = 0, .pc = 0x4, .regs = {0, 0, 0}, .ram_state = 0},
    {.cycle = 1, .pc = 0x8, .regs = {0, 0, 0xA}, .ram_state = 0},
    {.cycle = 2, .pc = 0xc, .regs = {0, 0xA, 0xA}, .ram_state = 0},
    {.cycle = 4, .pc = 0x10, .regs = {0, 0xA, 0x9}, .ram_state = 0},
    {.cycle = 5, .pc = 0x8, .regs = {0, 0xA, 0x9}, .ram_state = 0},
    {.cycle = 6, .pc = 0xc, .regs = {0, 0x13, 0x9}, .ram_state = 0},
    {.cycle = 7, .pc = 0x10, .regs = {0, 0x13, 0x8}, .ram_state = 0},
    {.cycle = 8, .pc = 0x8, .regs = {0, 0x13, 0x8}, .ram_state = 0},
    {.cycle = 9, .pc = 0xc, .regs = {0, 0x1b, 0x8}, .ram_state = 0},
    {.cycle = 10, .pc = 0x10, .regs = {0, 0x1b, 0x7}, .ram_state = 0},
    {.cycle = 11, .pc = 0x8, .regs = {0, 0x1b, 0x7}, .ram_state = 0},
    {.cycle = 12, .pc = 0xc, .regs = {0, 0x22, 0x7}, .ram_state = 0},
    {.cycle = 13, .pc = 0x10, .regs = {0, 0x22, 0x6}, .ram_state = 0},
    {.cycle = 14, .pc = 0x8, .regs = {0, 0x22, 0x6}, .ram_state = 0},
    {.cycle = 15, .pc = 0xc, .regs = {0, 0x28, 0x6}, .ram_state = 0},
    {.cycle = 16, .pc = 0x10, .regs = {0, 0x28, 0x5}, .ram_state = 0},
    {.cycle = 17, .pc = 0x8, .regs = {0, 0x28, 0x5}, .ram_state = 0},
    {.cycle = 18, .pc = 0xc, .regs = {0, 0x2d, 0x5}, .ram_state = 0},
    {.cycle = 19, .pc = 0x10, .regs = {0, 0x2d, 0x4}, .ram_state = 0},
    {.cycle = 20, .pc = 0x8, .regs = {0, 0x2d, 0x4}, .ram_state = 0},
    {.cycle = 21, .pc = 0xc, .regs = {0, 0x31, 0x4}, .ram_state = 0},
    {.cycle = 22, .pc = 0x10, .regs = {0, 0x31, 0x3}, .ram_state = 0},
    {.cycle = 23, .pc = 0x8, .regs = {0, 0x31, 0x3}, .ram_state = 0},
    {.cycle = 24, .pc = 0xc, .regs = {0, 0x34, 0x3}, .ram_state = 0},
    {.cycle = 25, .pc = 0x10, .regs = {0, 0x34, 0x2}, .ram_state = 0},
    {.cycle = 26, .pc = 0x8, .regs = {0, 0x34, 0x2}, .ram_state = 0},
    {.cycle = 27, .pc = 0xc, .regs = {0, 0x36, 0x2}, .ram_state = 0},
    {.cycle = 28, .pc = 0x10, .regs = {0, 0x36, 0x1}, .ram_state = 0},
    {.cycle = 29, .pc = 0x8, .regs = {0, 0x36, 0x1}, .ram_state = 0},
    {.cycle = 30, .pc = 0xc, .regs = {0, 0x37, 0x1}, .ram_state = 0},
    {.cycle = 31, .pc = 0x10, .regs = {0, 0x37, 0x0}, .ram_state = 0},
    {.cycle = 32, .pc = 0x14, .regs = {0, 0x37, 0x0}, .ram_state = 0},
    {.cycle = 33, .pc = 0x18, .regs = {0, 0x37, 0x0}, .ram_state = 0},
    {.cycle = 34, .pc = 0x1c, .regs = {0, 0x37, 0x0}, .ram_state = 0x37},
    {.cycle = 35,
     .pc = 0x20,
     .regs = {0, 0x37, 0x0, 0x0, 0x0, 0x37},
     .ram_state = 0x37},
    {.cycle = 36,
     .pc = 0x30,
     .regs = {0, 0x37, 0x0, 0x0, 0x0, 0x37},
     .ram_state = 0x37},
    {.cycle = 37,
     .pc = 0x34,
     .regs = {0, 0x37, 0x0, 0x0, 0x0, 0x3c},
     .ram_state = 0x37},
    {.cycle = 38,
     .pc = 0x34,
     .regs = {0, 0x37, 0x0, 0x0, 0x0, 0x3c},
     .ram_state = 0x37},
};

int main(int argc, char **argv) {
  TestBench<Vmain> tb(argc, argv, "sim/cpu_waveform.vcd");
  RAM ram;

  std::cout << "=== CPU Tests ===" << std::endl;

  tb.dut->enable = 1;
  tb.reset();

  for (int i = 0; i < 38; i++) {
    cpu_tick(tb, ram);

    uint32_t x0 = tb.dut->rootp->main__DOT__regfile__DOT__regs[0];
    uint32_t x1 = tb.dut->rootp->main__DOT__regfile__DOT__regs[1];
    uint32_t x2 = tb.dut->rootp->main__DOT__regfile__DOT__regs[2];
    uint32_t x5 = tb.dut->rootp->main__DOT__regfile__DOT__regs[5];
    uint32_t pc = tb.dut->rootp->main__DOT__PC_Address;

    tb.check_hex(std::format("Cycle {}th value of pc:   ", c[i].cycle), pc,
                 c[i].pc);
    tb.check_hex(std::format("Cycle {}th value of x0:   ", c[i].cycle), x0,
                 c[i].regs[0]);
    tb.check_hex(std::format("Cycle {}th value of x1:   ", c[i].cycle), x1,
                 c[i].regs[1]);
    tb.check_hex(std::format("Cycle {}th value of x2:   ", c[i].cycle), x2,
                 c[i].regs[2]);
    tb.check_hex(std::format("Cycle {}th value of x5:   ", c[i].cycle), x5,
                 c[i].regs[5]);
    tb.check_hex(std::format("Cycle {}th value of RAM[0]:", c[i].cycle),
                 ram.read(0), c[i].ram_state);
  }

  tb.show_summary();
  return tb.fail_count;
}

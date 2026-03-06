#pragma once
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define DEFAULT_TIME_STEP 10

template <class Module> class TestBench {
public:
  std::unique_ptr<VerilatedContext> ctx;
  Module *dut;
  VerilatedVcdC *tfp;
  int pass_count = 0;
  int fail_count = 0;

  TestBench(int argc, char **argv, const char *vcd_file) {
    ctx = std::make_unique<VerilatedContext>();
    ctx->commandArgs(argc, argv);
    dut = new Module{ctx.get()};

    ctx->traceEverOn(true);
    tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open(vcd_file);
  }

  ~TestBench() {
    tfp->close();
    delete dut;
  }

  void tick(int time_steps = DEFAULT_TIME_STEP) { ctx->timeInc(time_steps); }

  // ── Standard Decimal Check ──────────────────────────────────────
  template <typename T_GOT, typename T_EXP>
  int check(const char *signal, T_GOT got, T_EXP expected, int width = 30) {
    uint32_t got_val = static_cast<uint32_t>(got);
    uint32_t exp_val = static_cast<uint32_t>(expected);

    if (got_val == exp_val) {
      std::cout << std::format("  PASS {:<{}} = {}\n", signal, width, got_val);
      pass_count++;
      return 0;
    } else {
      std::cout << std::format("  [@ {} ps] FAIL {:<{}} got={} expected={}\n",
                               ctx->time(), signal, width, got_val, exp_val);
      fail_count++;
      return 1;
    }
  }

  // ── Hexadecimal Check ───────────────────────────────────────────
  template <typename T_GOT, typename T_EXP>
  int check_hex(const char *signal, T_GOT got, T_EXP expected, int width = 30) {
    uint32_t got_val = static_cast<uint32_t>(got);
    uint32_t exp_val = static_cast<uint32_t>(expected);

    if (got_val == exp_val) {
      std::cout << std::format("  PASS {:<{}} = {:#010x}\n", signal, width,
                               got_val);
      pass_count++;
      return 0;
    } else {
      std::cout << std::format(
          "  [@ {} ps] FAIL {:<{}} got={:#010x} expected={:#010x}\n",
          ctx->time(), signal, width, got_val, exp_val);
      fail_count++;
      return 1;
    }
  }

  // ── Print Final Results ─────────────────────────────────────────
  void show_summary() {
    std::cout << std::format("\n=== Summary ===\nPASS: {}\nFAIL: {}\n",
                             pass_count, fail_count);
  }
};

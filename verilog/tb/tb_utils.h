#pragma once
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define DEFAULT_TIME_STEP 10

// ── C++20 concepts ────────────────────────────────────────────────
template <typename T>
concept HasClock = requires(T t) { t.clk; };

template <typename T>
concept HasAReset = requires(T t) { t.areset; };

template <class Module> class TestBench {
public:
  std::unique_ptr<VerilatedContext> ctx;
  Module *dut;
  VerilatedVcdC *tfp;
  int pass_count = 0;
  int fail_count = 0;

private:
  uint64_t last_dump_time = UINT64_MAX; // sentinel - never dumped yet

  void safe_dump() {
    if (ctx->time() != last_dump_time) {
      tfp->dump(ctx->time());
      last_dump_time = ctx->time();
    }
  }

public:
  // ── Constructor ────────────────────────────────────────────────
  TestBench(int argc, char **argv, const char *vcd_file) {
    ctx = std::make_unique<VerilatedContext>();
    ctx->commandArgs(argc, argv);
    dut = new Module{ctx.get()};

    // initialize clock low if module has one
    if constexpr (HasClock<Module>) {
      dut->clk = 0;
    }

    ctx->traceEverOn(true);
    tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open(vcd_file);

    // initial eval and dump
    dut->eval();
    safe_dump();
    ctx->timeInc(DEFAULT_TIME_STEP);
  }

  // ── Destructor ─────────────────────────────────────────────────
  ~TestBench() {
    tfp->close();
    delete dut;
  }

  void half_tick(int time_steps = DEFAULT_TIME_STEP) {
    if constexpr (HasClock<Module>) {
      dut->clk = !dut->clk;
      dut->eval();
      safe_dump();
      ctx->timeInc(time_steps / 2);
    } else {
      dut->eval();
      safe_dump();
      ctx->timeInc(time_steps / 2);
    }
  }

  // ── Full Clock Cycle ───────────────────────────────────────────
  void tick(int time_steps = DEFAULT_TIME_STEP) {
    if constexpr (HasClock<Module>) {
      half_tick(time_steps);
      half_tick(time_steps);
    } else {
      dut->eval();
      safe_dump();
      ctx->timeInc(time_steps);
    }
  }

  // ── Reset ──────────────────────────────────────────────────────
  void reset(int time_steps = DEFAULT_TIME_STEP) {
    if constexpr (HasClock<Module> && HasAReset<Module>) {
      dut->areset = 1;
      tick(time_steps);
      dut->areset = 0;
    }
  }

  // ── Standard Decimal Check ─────────────────────────────────────
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

  // ── Hexadecimal Check ──────────────────────────────────────────
  template <typename T_GOT, typename T_EXP>
  int check_hex(std::string signal, T_GOT got, T_EXP expected, int width = 30) {
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

  // ── Binary Check ──────────────────────────────────────────────
  template <typename T_GOT, typename T_EXP>
  int check_bin(const char *signal, T_GOT got, T_EXP expected, int width = 30) {
    uint32_t got_val = static_cast<uint32_t>(got);
    uint32_t exp_val = static_cast<uint32_t>(expected);

    if (got_val == exp_val) {
      std::cout << std::format("  PASS {:<{}} = {:08b}\n", signal, width,
                               got_val);
      pass_count++;
      return 0;
    } else {
      std::cout << std::format(
          "  [@ {} ps] FAIL {:<{}} got={:08b} expected={:08b}\n", ctx->time(),
          signal, width, got_val, exp_val);
      fail_count++;
      return 1;
    }
  }

  // ── Print Final Results ────────────────────────────────────────
  void show_summary() {
    std::cout << std::format("\n=== Summary ===\nPASS: {}\nFAIL: {}\n",
                             pass_count, fail_count);
  }
};

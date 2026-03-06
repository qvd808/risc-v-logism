cmake_minimum_required(VERSION 3.16)
project(riscv_cpu)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Verilator
find_package(verilator HINTS $ENV{VERILATOR_ROOT})
if (NOT verilator_FOUND)
    message(FATAL_ERROR "Verilator not found!")
endif()

set(CMAKE_CXX_STANDARD 17)

# ──────────────────────────────
# ALU test
# ──────────────────────────────
add_executable(alu_sim tb/tb_ALU.cpp)

verilate(alu_sim
    SOURCES rtl/ALU.v
    TOP_MODULE ALU
    TRACE  # Trace waveform
)

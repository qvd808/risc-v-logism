cmake_minimum_required(VERSION 3.16)
project(riscv_cpu)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Force C++20 globally for standard CMake targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Force C++20 for Verilator's internal GCC/Clang calls
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# Find Verilator
find_package(verilator HINTS $ENV{VERILATOR_ROOT})
if (NOT verilator_FOUND)
    message(FATAL_ERROR "Verilator not found!")
endif()

# ──────────────────────────────
# ALU test
# ──────────────────────────────
add_executable(alu_sim tb/tb_ALU.cpp tb/tb_utils.h)

target_include_directories(alu_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tb)

verilate(alu_sim
    SOURCES rtl/ALU.v
    TOP_MODULE ALU
    TRACE  # Trace waveform
)

# ──────────────────────────────
# Control Unit test
# ──────────────────────────────
add_executable(cu_sim tb/tb_ControlUnit.cpp tb/tb_utils.h)

target_include_directories(cu_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tb)

verilate(cu_sim
    SOURCES
        rtl/ControlUnit.sv
        assertions/ControlUnit_assert.sv
        assertions/bind_ControlUnit.sv
    TOP_MODULE ControlUnit
    TRACE
    VERILATOR_ARGS --sv --assert
)

# ──────────────────────────────
# ALU Control Unit test
# ──────────────────────────────
add_executable(alucu_sim tb/tb_ALUControlUnit.cpp tb/tb_utils.h)

target_include_directories(alucu_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tb)

verilate(alucu_sim
    SOURCES
		rtl/ALUControlUnit.sv
	TOP_MODULE ALUControlUnit
    TRACE
    VERILATOR_ARGS --sv --assert
)


# ──────────────────────────────
# ImmGen Unit test
# ──────────────────────────────
add_executable(immgen_sim tb/tb_ImmGen.cpp tb/tb_utils.h)

target_include_directories(immgen_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tb)

verilate(immgen_sim
    SOURCES
		rtl/ImmGen.sv
	TOP_MODULE ImmGen
    TRACE
    VERILATOR_ARGS --sv --assert
)


# ──────────────────────────────
# PC Unit test
# ──────────────────────────────
add_executable(pc_sim tb/tb_PC.cpp tb/tb_utils.h)

target_include_directories(pc_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tb)

verilate(pc_sim
    SOURCES
		rtl/PC.sv
	TOP_MODULE PC
    TRACE
    VERILATOR_ARGS --sv --assert
)


# ──────────────────────────────
# RegFile Unit test
# ──────────────────────────────
add_executable(regfile_sim tb/tb_RegFile.cpp tb/tb_utils.h)

target_include_directories(regfile_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tb)

verilate(regfile_sim
    SOURCES
		rtl/RegFile.sv
	TOP_MODULE RegFile
    TRACE
    VERILATOR_ARGS --sv --assert
)

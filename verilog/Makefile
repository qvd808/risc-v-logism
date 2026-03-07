# Makefile
BUILD_DIR = build
SIM_DIR   = sim

$(BUILD_DIR)/Makefile: CMakeLists.txt
	@mkdir -p $(SIM_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

all: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) -j$(nproc)

alu: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target alu_sim -j$(nproc)
	@./$(BUILD_DIR)/alu_sim

cu: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target cu_sim -j$(nproc)
	@./$(BUILD_DIR)/cu_sim

alucu: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target alucu_sim -j$(nproc)
	@./$(BUILD_DIR)/alucu_sim

immgen: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target immgen_sim -j$(nproc)
	@./$(BUILD_DIR)/immgen_sim

pc: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target pc_sim -j$(nproc)
	@./$(BUILD_DIR)/pc_sim

regfile: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target regfile_sim -j$(nproc)
	@./$(BUILD_DIR)/regfile_sim

cpu: $(BUILD_DIR)/Makefile
	@cmake --build $(BUILD_DIR) --target main -j$(nproc)
	@./$(BUILD_DIR)/main

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory removed"

cleanall:
	@rm -rf $(BUILD_DIR) $(SIM_DIR)
	@echo "Build and sim directories removed"

.PHONY: all alu cu alucu immgen pc regfile cpu clean cleanall

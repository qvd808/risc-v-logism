# Makefile
BUILD_DIR = build
SIM_DIR   = sim

# build everything
all:
	@mkdir -p $(SIM_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR)

# run individual modules
alu:
	@mkdir -p $(SIM_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR) --target alu_sim
	@./$(BUILD_DIR)/alu_sim

cu:
	@mkdir -p $(SIM_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR) --target cu_sim
	@./$(BUILD_DIR)/cu_sim

alucu:
	@mkdir -p $(SIM_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR) --target alucu_sim
	@./$(BUILD_DIR)/alucu_sim

immgen:
	@mkdir -p $(SIM_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR) --target immgen_sim
	@./$(BUILD_DIR)/immgen_sim

# clean just build artifacts
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory removed"

# clean everything including sim outputs
cleanall:
	@rm -rf $(BUILD_DIR) $(SIM_DIR)
	@echo "Build and sim directories removed"

.PHONY: all alu cu alucu clean cleanall

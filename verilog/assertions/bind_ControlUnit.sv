// file: assertions/bind_ControlUnit.sv

// syntax:
// bind <who to attach to> <what to attach> <instance name> (port connections)
bind ControlUnit ControlUnit_assert u_assert (
    // .<assert_module_port> (<controlunit_signal>)
    .instruction(instruction),
    .ALUOps     (ALUOps),
    .ALUSrc     (ALUSrc),
    .MemWrite   (MemWrite),
    .RegWrite   (RegWrite),
    .MemRead    (MemRead),
    .MemtoReg   (MemtoReg),
    .Branch     (Branch),
    .Jump       (Jump)
);

// syntax:
// bind <who to attach to> <what to attach> <instance name> (port connections)
bind PC PC_assert u_assert (
    // .<assert_module_port> (<controlunit_signal>)
    .clk            (clk),
    .PCEnable       (PCEnable),
    .areset         (areset),
    .address_to_load(address_to_load),
    .load_address   (load_address),
    .PC_Address     (PC_Address)
);

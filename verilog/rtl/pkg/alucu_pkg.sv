package alucu_pkg;
  typedef enum logic [1:0] {
    ALUOP_ADD = 2'b00,  // always add
    ALUOP_SUB = 2'b01,  // always sub
    ALUOP_FUNC = 2'b10,  // decode with funct7, func3
    ALUOP_RESERVE = 2'b11  // reserve
  } aluop_t  /*verilator public*/;  // <-- Add this to get defines in C++
endpackage

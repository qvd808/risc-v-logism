module main (
    input  wire       CLOCK_50, // 50MHz Clock
    input  wire [0:0] KEY,      
	 
	 // HPS DDR3 Pins
    output wire [14:0] HPS_DDR3_ADDR,
    output wire [2:0]  HPS_DDR3_BA,
    output wire        HPS_DDR3_CAS_N,
    output wire        HPS_DDR3_CKE,
    output wire        HPS_DDR3_CK_N,
    output wire        HPS_DDR3_CK_P,
    output wire        HPS_DDR3_CS_N,
    output wire [3:0]  HPS_DDR3_DM,
    inout  wire [31:0] HPS_DDR3_DQ,
    inout  wire [3:0]  HPS_DDR3_DQS_N,
    inout  wire [3:0]  HPS_DDR3_DQS_P,
    output wire        HPS_DDR3_ODT,
    output wire        HPS_DDR3_RAS_N,
    output wire        HPS_DDR3_RESET_N,
    input  wire        HPS_DDR3_RZQ,
    output wire        HPS_DDR3_WE_N,

    // HPS Minimum I/O
    output wire        HPS_UART_TX,
    input  wire        HPS_UART_RX,
    output wire        HPS_SD_CLK,
    inout  wire        HPS_SD_CMD,
    inout  wire [3:0]  HPS_SD_DATA,

    // FPGA LEDs
    output wire [9:0]  LEDR
);

    wire [31:0] hps_to_fpga_data;

    // --- Minimalist System Instantiation ---
    soc_system u0 (
        .clk_clk                           (CLOCK_50),
        .reset_reset_n                     (KEY[0]),

        // DDR3 Interface
        .memory_mem_a                      (HPS_DDR3_ADDR),
        .memory_mem_ba                     (HPS_DDR3_BA),
        .memory_mem_ck                     (HPS_DDR3_CK_P),
        .memory_mem_ck_n                   (HPS_DDR3_CK_N),
        .memory_mem_cke                    (HPS_DDR3_CKE),
        .memory_mem_cs_n                   (HPS_DDR3_CS_N),
        .memory_mem_ras_n                  (HPS_DDR3_RAS_N),
        .memory_mem_cas_n                  (HPS_DDR3_CAS_N),
        .memory_mem_we_n                   (HPS_DDR3_WE_N),
        .memory_mem_reset_n                (HPS_DDR3_RESET_N),
        .memory_mem_dq                     (HPS_DDR3_DQ),
        .memory_mem_dqs                    (HPS_DDR3_DQS_P),
        .memory_mem_dqs_n                  (HPS_DDR3_DQS_N),
        .memory_mem_odt                    (HPS_DDR3_ODT),
        .memory_mem_dm                     (HPS_DDR3_DM),
        .memory_oct_rzqin                  (HPS_DDR3_RZQ),

        // HPS I/O (SD Card & UART)
        .hps_io_hps_io_sdio_inst_CLK       (HPS_SD_CLK),
        .hps_io_hps_io_sdio_inst_CMD       (HPS_SD_CMD),
        .hps_io_hps_io_sdio_inst_D0        (HPS_SD_DATA[0]),
        .hps_io_hps_io_uart0_inst_RX       (HPS_UART_RX),
        .hps_io_hps_io_uart0_inst_TX       (HPS_UART_TX),

        // BRAM Test Port
        .onchip_memory2_0_s2_address       (13'h0),
        .onchip_memory2_0_s2_chipselect    (1'b1),
        .onchip_memory2_0_s2_clken         (1'b1),
        .onchip_memory2_0_s2_write         (1'b0),
        .onchip_memory2_0_s2_readdata      (hps_to_fpga_data),
        .onchip_memory2_0_s2_byteenable    (4'hF)
    );

    // If data at address 0 is '2', LED 2 lights up!
    assign LEDR[2] = (hps_to_fpga_data == 32'd2);
    assign LEDR[9:3] = 7'b0;
    assign LEDR[1:0] = 2'b0;

endmodule

module soc_system (
	clk_clk,
	hps_0_h2f_mpu_events_eventi,
	hps_0_h2f_mpu_events_evento,
	hps_0_h2f_mpu_events_standbywfe,
	hps_0_h2f_mpu_events_standbywfi,
	hps_io_hps_io_uart0_inst_RX,
	hps_io_hps_io_uart0_inst_TX,
	memory_mem_a,
	memory_mem_ba,
	memory_mem_ck,
	memory_mem_ck_n,
	memory_mem_cke,
	memory_mem_cs_n,
	memory_mem_ras_n,
	memory_mem_cas_n,
	memory_mem_we_n,
	memory_mem_reset_n,
	memory_mem_dq,
	memory_mem_dqs,
	memory_mem_dqs_n,
	memory_mem_odt,
	memory_mem_dm,
	memory_oct_rzqin,
	onchip_memory2_0_s2_address,
	onchip_memory2_0_s2_chipselect,
	onchip_memory2_0_s2_clken,
	onchip_memory2_0_s2_write,
	onchip_memory2_0_s2_readdata,
	onchip_memory2_0_s2_writedata,
	onchip_memory2_0_s2_byteenable,
	onchip_memory2_1_s2_address,
	onchip_memory2_1_s2_chipselect,
	onchip_memory2_1_s2_clken,
	onchip_memory2_1_s2_write,
	onchip_memory2_1_s2_readdata,
	onchip_memory2_1_s2_writedata,
	onchip_memory2_1_s2_byteenable,
	reset_reset_n);	

	input		clk_clk;
	input		hps_0_h2f_mpu_events_eventi;
	output		hps_0_h2f_mpu_events_evento;
	output	[1:0]	hps_0_h2f_mpu_events_standbywfe;
	output	[1:0]	hps_0_h2f_mpu_events_standbywfi;
	input		hps_io_hps_io_uart0_inst_RX;
	output		hps_io_hps_io_uart0_inst_TX;
	output	[12:0]	memory_mem_a;
	output	[2:0]	memory_mem_ba;
	output		memory_mem_ck;
	output		memory_mem_ck_n;
	output		memory_mem_cke;
	output		memory_mem_cs_n;
	output		memory_mem_ras_n;
	output		memory_mem_cas_n;
	output		memory_mem_we_n;
	output		memory_mem_reset_n;
	inout	[7:0]	memory_mem_dq;
	inout		memory_mem_dqs;
	inout		memory_mem_dqs_n;
	output		memory_mem_odt;
	output		memory_mem_dm;
	input		memory_oct_rzqin;
	input	[12:0]	onchip_memory2_0_s2_address;
	input		onchip_memory2_0_s2_chipselect;
	input		onchip_memory2_0_s2_clken;
	input		onchip_memory2_0_s2_write;
	output	[31:0]	onchip_memory2_0_s2_readdata;
	input	[31:0]	onchip_memory2_0_s2_writedata;
	input	[3:0]	onchip_memory2_0_s2_byteenable;
	input	[11:0]	onchip_memory2_1_s2_address;
	input		onchip_memory2_1_s2_chipselect;
	input		onchip_memory2_1_s2_clken;
	input		onchip_memory2_1_s2_write;
	output	[31:0]	onchip_memory2_1_s2_readdata;
	input	[31:0]	onchip_memory2_1_s2_writedata;
	input	[3:0]	onchip_memory2_1_s2_byteenable;
	input		reset_reset_n;
endmodule

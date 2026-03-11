	component soc_system is
		port (
			clk_clk                         : in    std_logic                     := 'X';             -- clk
			hps_0_h2f_mpu_events_eventi     : in    std_logic                     := 'X';             -- eventi
			hps_0_h2f_mpu_events_evento     : out   std_logic;                                        -- evento
			hps_0_h2f_mpu_events_standbywfe : out   std_logic_vector(1 downto 0);                     -- standbywfe
			hps_0_h2f_mpu_events_standbywfi : out   std_logic_vector(1 downto 0);                     -- standbywfi
			hps_io_hps_io_uart0_inst_RX     : in    std_logic                     := 'X';             -- hps_io_uart0_inst_RX
			hps_io_hps_io_uart0_inst_TX     : out   std_logic;                                        -- hps_io_uart0_inst_TX
			memory_mem_a                    : out   std_logic_vector(12 downto 0);                    -- mem_a
			memory_mem_ba                   : out   std_logic_vector(2 downto 0);                     -- mem_ba
			memory_mem_ck                   : out   std_logic;                                        -- mem_ck
			memory_mem_ck_n                 : out   std_logic;                                        -- mem_ck_n
			memory_mem_cke                  : out   std_logic;                                        -- mem_cke
			memory_mem_cs_n                 : out   std_logic;                                        -- mem_cs_n
			memory_mem_ras_n                : out   std_logic;                                        -- mem_ras_n
			memory_mem_cas_n                : out   std_logic;                                        -- mem_cas_n
			memory_mem_we_n                 : out   std_logic;                                        -- mem_we_n
			memory_mem_reset_n              : out   std_logic;                                        -- mem_reset_n
			memory_mem_dq                   : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- mem_dq
			memory_mem_dqs                  : inout std_logic                     := 'X';             -- mem_dqs
			memory_mem_dqs_n                : inout std_logic                     := 'X';             -- mem_dqs_n
			memory_mem_odt                  : out   std_logic;                                        -- mem_odt
			memory_mem_dm                   : out   std_logic;                                        -- mem_dm
			memory_oct_rzqin                : in    std_logic                     := 'X';             -- oct_rzqin
			onchip_memory2_0_s2_address     : in    std_logic_vector(12 downto 0) := (others => 'X'); -- address
			onchip_memory2_0_s2_chipselect  : in    std_logic                     := 'X';             -- chipselect
			onchip_memory2_0_s2_clken       : in    std_logic                     := 'X';             -- clken
			onchip_memory2_0_s2_write       : in    std_logic                     := 'X';             -- write
			onchip_memory2_0_s2_readdata    : out   std_logic_vector(31 downto 0);                    -- readdata
			onchip_memory2_0_s2_writedata   : in    std_logic_vector(31 downto 0) := (others => 'X'); -- writedata
			onchip_memory2_0_s2_byteenable  : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- byteenable
			onchip_memory2_1_s2_address     : in    std_logic_vector(11 downto 0) := (others => 'X'); -- address
			onchip_memory2_1_s2_chipselect  : in    std_logic                     := 'X';             -- chipselect
			onchip_memory2_1_s2_clken       : in    std_logic                     := 'X';             -- clken
			onchip_memory2_1_s2_write       : in    std_logic                     := 'X';             -- write
			onchip_memory2_1_s2_readdata    : out   std_logic_vector(31 downto 0);                    -- readdata
			onchip_memory2_1_s2_writedata   : in    std_logic_vector(31 downto 0) := (others => 'X'); -- writedata
			onchip_memory2_1_s2_byteenable  : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- byteenable
			reset_reset_n                   : in    std_logic                     := 'X'              -- reset_n
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			clk_clk                         => CONNECTED_TO_clk_clk,                         --                  clk.clk
			hps_0_h2f_mpu_events_eventi     => CONNECTED_TO_hps_0_h2f_mpu_events_eventi,     -- hps_0_h2f_mpu_events.eventi
			hps_0_h2f_mpu_events_evento     => CONNECTED_TO_hps_0_h2f_mpu_events_evento,     --                     .evento
			hps_0_h2f_mpu_events_standbywfe => CONNECTED_TO_hps_0_h2f_mpu_events_standbywfe, --                     .standbywfe
			hps_0_h2f_mpu_events_standbywfi => CONNECTED_TO_hps_0_h2f_mpu_events_standbywfi, --                     .standbywfi
			hps_io_hps_io_uart0_inst_RX     => CONNECTED_TO_hps_io_hps_io_uart0_inst_RX,     --               hps_io.hps_io_uart0_inst_RX
			hps_io_hps_io_uart0_inst_TX     => CONNECTED_TO_hps_io_hps_io_uart0_inst_TX,     --                     .hps_io_uart0_inst_TX
			memory_mem_a                    => CONNECTED_TO_memory_mem_a,                    --               memory.mem_a
			memory_mem_ba                   => CONNECTED_TO_memory_mem_ba,                   --                     .mem_ba
			memory_mem_ck                   => CONNECTED_TO_memory_mem_ck,                   --                     .mem_ck
			memory_mem_ck_n                 => CONNECTED_TO_memory_mem_ck_n,                 --                     .mem_ck_n
			memory_mem_cke                  => CONNECTED_TO_memory_mem_cke,                  --                     .mem_cke
			memory_mem_cs_n                 => CONNECTED_TO_memory_mem_cs_n,                 --                     .mem_cs_n
			memory_mem_ras_n                => CONNECTED_TO_memory_mem_ras_n,                --                     .mem_ras_n
			memory_mem_cas_n                => CONNECTED_TO_memory_mem_cas_n,                --                     .mem_cas_n
			memory_mem_we_n                 => CONNECTED_TO_memory_mem_we_n,                 --                     .mem_we_n
			memory_mem_reset_n              => CONNECTED_TO_memory_mem_reset_n,              --                     .mem_reset_n
			memory_mem_dq                   => CONNECTED_TO_memory_mem_dq,                   --                     .mem_dq
			memory_mem_dqs                  => CONNECTED_TO_memory_mem_dqs,                  --                     .mem_dqs
			memory_mem_dqs_n                => CONNECTED_TO_memory_mem_dqs_n,                --                     .mem_dqs_n
			memory_mem_odt                  => CONNECTED_TO_memory_mem_odt,                  --                     .mem_odt
			memory_mem_dm                   => CONNECTED_TO_memory_mem_dm,                   --                     .mem_dm
			memory_oct_rzqin                => CONNECTED_TO_memory_oct_rzqin,                --                     .oct_rzqin
			onchip_memory2_0_s2_address     => CONNECTED_TO_onchip_memory2_0_s2_address,     --  onchip_memory2_0_s2.address
			onchip_memory2_0_s2_chipselect  => CONNECTED_TO_onchip_memory2_0_s2_chipselect,  --                     .chipselect
			onchip_memory2_0_s2_clken       => CONNECTED_TO_onchip_memory2_0_s2_clken,       --                     .clken
			onchip_memory2_0_s2_write       => CONNECTED_TO_onchip_memory2_0_s2_write,       --                     .write
			onchip_memory2_0_s2_readdata    => CONNECTED_TO_onchip_memory2_0_s2_readdata,    --                     .readdata
			onchip_memory2_0_s2_writedata   => CONNECTED_TO_onchip_memory2_0_s2_writedata,   --                     .writedata
			onchip_memory2_0_s2_byteenable  => CONNECTED_TO_onchip_memory2_0_s2_byteenable,  --                     .byteenable
			onchip_memory2_1_s2_address     => CONNECTED_TO_onchip_memory2_1_s2_address,     --  onchip_memory2_1_s2.address
			onchip_memory2_1_s2_chipselect  => CONNECTED_TO_onchip_memory2_1_s2_chipselect,  --                     .chipselect
			onchip_memory2_1_s2_clken       => CONNECTED_TO_onchip_memory2_1_s2_clken,       --                     .clken
			onchip_memory2_1_s2_write       => CONNECTED_TO_onchip_memory2_1_s2_write,       --                     .write
			onchip_memory2_1_s2_readdata    => CONNECTED_TO_onchip_memory2_1_s2_readdata,    --                     .readdata
			onchip_memory2_1_s2_writedata   => CONNECTED_TO_onchip_memory2_1_s2_writedata,   --                     .writedata
			onchip_memory2_1_s2_byteenable  => CONNECTED_TO_onchip_memory2_1_s2_byteenable,  --                     .byteenable
			reset_reset_n                   => CONNECTED_TO_reset_reset_n                    --                reset.reset_n
		);


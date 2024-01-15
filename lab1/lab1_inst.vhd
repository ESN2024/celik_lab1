	component lab1 is
		port (
			buttons_external_connection_export : in  std_logic_vector(1 downto 0) := (others => 'X'); -- export
			clk_clk                            : in  std_logic                    := 'X';             -- clk
			led_external_connection_export     : out std_logic_vector(7 downto 0);                    -- export
			reset_reset_n                      : in  std_logic                    := 'X';             -- reset_n
			switch_external_connection_export  : in  std_logic_vector(7 downto 0) := (others => 'X')  -- export
		);
	end component lab1;

	u0 : component lab1
		port map (
			buttons_external_connection_export => CONNECTED_TO_buttons_external_connection_export, -- buttons_external_connection.export
			clk_clk                            => CONNECTED_TO_clk_clk,                            --                         clk.clk
			led_external_connection_export     => CONNECTED_TO_led_external_connection_export,     --     led_external_connection.export
			reset_reset_n                      => CONNECTED_TO_reset_reset_n,                      --                       reset.reset_n
			switch_external_connection_export  => CONNECTED_TO_switch_external_connection_export   --  switch_external_connection.export
		);


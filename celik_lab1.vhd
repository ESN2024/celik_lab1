library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity celik_lab1 is 
		port (
			clk : in std_logic := '0';
			reset_n : in std_logic :='0';
			SW  : in  std_logic_vector(7 downto 0);
			Buttons  : in  std_logic_vector(1 downto 0);
			LED : out std_logic_vector(7 downto 0));
end entity;


architecture arch of celik_lab1 is
    component lab1 is
        port (
            clk_clk                           	: in  std_logic                    := 'X';             			-- clk
            led_external_connection_export    	: out std_logic_vector(7 downto 0);                    			-- export
            reset_reset_n                     	: in  std_logic                    := 'X';             			-- reset_n
            switch_external_connection_export 	: in  std_logic_vector(7 downto 0) := (others => 'X');  			-- export
				buttons_external_connection_export 	: in  std_logic_vector(1 downto 0) := (others => 'X') 			-- export
        );
    end component lab1;

	 
begin
    u0 : component lab1
        port map (
            clk_clk                           	=> clk,                           										-- clk.clk
            led_external_connection_export    	=> LED,    																		-- led_external_connection.export
            reset_reset_n                     	=> reset_n,                     											-- reset.reset_n
            switch_external_connection_export 	=> SW,  																			-- switch_external_connection.export
				buttons_external_connection_export 	=> Buttons  																	-- buttons_external_connection.export
        );
end architecture;


module lab1 (
	buttons_external_connection_export,
	clk_clk,
	led_external_connection_export,
	reset_reset_n,
	switch_external_connection_export);	

	input	[1:0]	buttons_external_connection_export;
	input		clk_clk;
	output	[7:0]	led_external_connection_export;
	input		reset_reset_n;
	input	[7:0]	switch_external_connection_export;
endmodule

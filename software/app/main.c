#include "system.h"
#include "altera_avalon_pio_regs.h"

int main(void)
{
	IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x01); // data 
	return 0;
}
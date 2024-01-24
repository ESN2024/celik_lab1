#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "io.h"
#include "unistd.h"
#include <stdio.h>
#include <stdint.h>

int main(void)
{
	int32_t addr = 0x01;
	int32_t time = 50000;
	
	alt_printf("Dans le main du led_chaser\n");
	IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr);
	usleep(time);
	
	while(1)
	{
		while(addr != 0x80)
		{
			addr = addr << 1;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr);			
			usleep(time);
		}
		while(addr != 0x01)
		{
			addr = addr >> 1;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr);
			usleep(time);
		}
	}
}
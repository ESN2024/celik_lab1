#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "io.h"
#include "unistd.h"

int main(void)
{
	int addr = 0x01;
	int time = 50000;
	int SW; // Switch
	
	alt_printf("Dans le main\n");
	IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr);
	usleep(time);
	
	while(1)
	{
		SW = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
		alt_printf("%x\n",SW);
		
		if(SW == 0x01)
		{
			time = 400000;
		}
		else if(SW == 0x02)
		{
			time = 300000;
		}
		else if(SW == 0x04)
		{
			time = 200000;
		}
		else if(SW == 0x08)
		{
			time = 100000;
		}
		else if(SW == 0x10)
		{
			time = 20000;
		}
		else if(SW == 0x20)
		{
			time = 15000;
		}
		else if(SW == 0x40)
		{
			time = 10000;
		}
		else if(SW == 0x80)
		{
			time = 5000;
		}
		else
		{
			time = 50000;
		}
		
		while(addr != 0x80)
		{
			addr = addr << 1;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr);
			usleep(time);
		}
		while(addr != 0x01) 
		{
			addr = addr >> 1;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE,addr);
			usleep(time);
		}
	}
}
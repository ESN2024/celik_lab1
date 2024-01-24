#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "io.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

/* Globales declarations variables */
uint8_t choix = 0;
uint8_t key;
uint8_t sw;
uint32_t time = 50000;
uint32_t addr1;
uint32_t addr2;

/* Key interrupt function */
static void key_interrupt(void *Context)
{
	alt_printf("Key interrupt\n");
	key = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);
	
	switch(key)
	{
		case 0b01: /* On arrete le led_chaser et on remet les led a 0 */
			choix = 0;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x00);
			addr1 = 0x80;
			addr2 = 0x01;
			alt_printf("%x\n", key);
			break;
			
		case 0b10: /* On lance le LED_chaser */
			choix = 1;
			alt_printf("%x\n", key);
			break;
			
		default:
			choix = 0;
			break;
	}

	/* Reset the key interrupt */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0b11);
}

int8_t nombre_bits_1(int x) 
{
	int8_t cpt = 0;
	
	while(x) 
	{
		cpt += x & 1;
		x >>= 1;
	}
	return cpt;
}

/* SW interrupt function */
static void sw_interrupt(void *Context)
{
	alt_printf("SW interrupt\n");
	sw = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
	uint8_t sw1 = nombre_bits_1(sw);
	time = sw1 * 20000 + 20000;

	/* Reset the sw interrupt */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0b1111111111);
}

/* Led chaser function */
void LED_chaser(void)
{
	IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr1);
	usleep(time);
	
	while(choix == 1)
	{
		while(addr1 != 0x80)
		{
			addr1 = addr1 << 1;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr1);
			usleep(time);
		}
		
		while(addr2 != 0x01)
		{
			addr2 = addr2 >> 1;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr2);
			usleep(time);
		}
		
		addr1 = 0x01;
		addr2 = 0x80;
	}
}

int main(void)
{
	alt_printf("Dans le main du led_chaser_interrupt\n");
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0b11);
	
	/* Applique un mask 0b1111111111 afin d'activer les switches */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCH_BASE, 0b1111111111);
	
	/* Active la detection des boutons */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0b11);
	
	/* Active la detection des switches */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0b1111111111);
	
	/* Creation de l'interruption pour les boutons */
	if(alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID,BUTTONS_IRQ, key_interrupt, NULL, NULL) != 0)
	{
		alt_printf("Error key interrupt\n");
	}
	
	/* Creation de l'interruption pour les switches */
	if(alt_ic_isr_register(SWITCH_IRQ_INTERRUPT_CONTROLLER_ID,SWITCH_IRQ, sw_interrupt, NULL, NULL) != 0)
	{
		alt_printf("Error sw interrupt\n");
	}
	
	while(1)
	{
		switch(choix)
		{
			case 0:
				break;
				
			case 1:
				addr1 = 0x01;
				addr2 = 0x80;
				LED_chaser();
				break;
		}
	}
}

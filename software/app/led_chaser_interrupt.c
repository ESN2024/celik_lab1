#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "io.h"
#include "unistd.h"

// Prototypes de fonctions
static void BP_interrupt(void *context); 
static void SW_interrupt(void *context);
int nombre_bits_1(int x);
void LED_chaser(void);

// Globales variables
int choix = 0;
int BP;
int SW;
int time = 50000;
int addr1;
int addr2;

int main(void)
{
	alt_printf("Dans le main\n");
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0b11);
	
	// Applique un mask 0b1111111111 afin d'activer les switches
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCH_BASE, 0b1111111111);
	
	// Active la detection des boutons
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0b11);
	
	// Active la detection des switches
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0b1111111111);
	
	// Creation de l'interruption pour les boutons
	if(alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID,BUTTONS_IRQ, BP_interrupt, NULL, NULL) != 0)
	{
		alt_printf("Error : creation BP_interrupt\n");
	}
	
	// Creation de l'interruption pour les switches
	if(alt_ic_isr_register(SWITCH_IRQ_INTERRUPT_CONTROLLER_ID,SWITCH_IRQ, SW_interrupt, NULL, NULL) != 0)
	{
		alt_printf("Error : creation SW_interrupt\n");
	}
	
	// LED_chaser
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

// BP_interrupt function
static void BP_interrupt(void *context)
{
	alt_printf("BP_interrupt function\n");
	BP = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);
	
	switch(BP)
	{
		case 0b01: // On arrete le led_chaser et on remet les led a 0
			choix = 0;
			IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x00);
			addr1 = 0x80;
			addr2 = 0x01;
			alt_printf("%x\n", BP);
			break;
			
		case 0b10: // On lance le LED_chaser
			choix = 1;
			alt_printf("%x\n", BP);
			break;
			
		default:
			choix = 0;
			break;
	}

	// Reset BP
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0b11);
}

// SW_interrupt function
static void SW_interrupt(void *context)
{
	alt_printf("SW_interrupt function\n");
	SW = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);
	int sw1 = nombre_bits_1(SW);
	time = sw1 * 20000 + 20000;

	// Reset SW 
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0b1111111111);
}

int nombre_bits_1(int x) 
{
	int cpt = 0;
	
	while(x) 
	{
		cpt += x & 1;
		x >>= 1;
	}
	return cpt;
}

// LED_chaser function
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
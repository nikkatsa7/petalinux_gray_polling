/*****************************************************************************/
/*                         I N C L U D E   F I L E S                         */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


/*****************************************************************************/
/*                            D E F I N E S                                  */
/*****************************************************************************/


#define SWITCH_BASE_ADDR 	0x41200000
#define BUTTONS_BASE_ADDR  	0x41210000
#define LEDS_BASE_ADDR		0x43C00000
#define GRAY_BASE_ADDR		0x43C10000



/*****************************************************************************/
/*                                M A I N                                    */
/*****************************************************************************/

int main(void){
	
	int 		i, psb_check, dip_check, counter,Data;
	int 		buttons_value = 0;
	int 		switch_value = 0;
	int 		fd;
	void 		*ptr_buttons,*ptr_switch,*ptr_gray,*ptr_led;
	unsigned 	page_addr_buttons,page_addr_switch,page_addr_gray,page_addr_led;
	unsigned	page_offset_buttons,page_offset_switch,page_offset_gray,page_offset_led;
	unsigned 	page_size = sysconf(_SC_PAGESIZE);
	
	fd = open("/dev/mem",O_RDWR);

	page_addr_buttons	= (BUTTONS_BASE_ADDR & ~(page_size-1));
	page_offset_buttons	= BUTTONS_BASE_ADDR - page_addr_buttons;

	page_addr_switch	= (SWITCH_BASE_ADDR & ~(page_size-1));
	page_offset_switch	= SWITCH_BASE_ADDR - page_addr_switch;

	page_addr_gray		= (GRAY_BASE_ADDR & ~(page_size-1));
	page_offset_gray	= GRAY_BASE_ADDR - page_addr_gray;

	page_addr_led		= (LEDS_BASE_ADDR & ~(page_size-1));
	page_offset_led		= LEDS_BASE_ADDR - page_addr_led;

	
	ptr_buttons = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(BUTTONS_BASE_ADDR & ~(page_size-1)));
	ptr_switch 	= mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(SWITCH_BASE_ADDR & ~(page_size-1)));
	ptr_gray 	= mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(GRAY_BASE_ADDR & ~(page_size-1)));
	ptr_led 	= mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(LEDS_BASE_ADDR & ~(page_size-1)));
	
	
	while (1)
   	{ 
		buttons_value = *((unsigned *)(ptr_buttons+page_offset_buttons));		
		fprintf(stdout,"Buttons Value %d\n", buttons_value);

		if(buttons_value == 0)
			  	Data = 0;
		else if(buttons_value == 2)
		  	  	Data = 1;
		else if(buttons_value == 16)
		  	 	Data = 2;
		else	Data = 3;
		

		fprintf(stdout,"Data: %d\n", Data);

		//GRAY_COUNTER_mWriteReg(XPAR_GRAY_COUNTER_S_AXI_BASEADDR, 0, Data);
		*((unsigned *)(ptr_gray+page_offset_gray))= Data;
		
		counter = *((unsigned *)(ptr_gray+page_offset_gray+4));

		fprintf(stdout,"Counter Status %d\n", counter);
		// output dip switches value on LED_ip device
		//LED_IP_mWriteReg(XPAR_LED_IP_S_AXI_BASEADDR, 0, counter);
		*((unsigned *)(ptr_led+page_offset_led)) = counter;


		for (i=0; i<9999999; i++);
   }	
	
	return 0;
}

/*
 * main.c
 */

//GPIO5
#define LED1 						0x00200000   // Bit 21
#define LED0 						0x00400000   // Bit 22
#define GPIO5_OUT					(*((volatile unsigned long*) 0x4905603C)) // Data Out register
#define GPIO5_OE 					(*((volatile unsigned long*) 0x49056034)) // Output Data Enable Register
//PRCM
#define CM_FCLKEN_PER				(*((volatile unsigned long*) 0x48005000)) // Controls the modules functional clock activity.
#define CM_ICLKEN_PER				(*((volatile unsigned long*) 0x48005010)) // Controls the modules interface clock activity
//SCM
#define CONTROL_PADCONF_UART1_TX	(*((volatile unsigned long*) 0x4800217c)) // Pad configuration for GPIO_149
#define CONTROL_PADCONF_UART1_CTS	(*((volatile unsigned long*) 0x48002180)) // Pad configuration for GPIO_150

void main(void) {

	long int i;
	// Set the pinmux to select the GPIO signal
	CONTROL_PADCONF_UART1_TX &= 0x0000FFFF; //  [31:16]=GPIO_149  - Clear register bits [31:16]
	CONTROL_PADCONF_UART1_TX |= 0x00040000; //  [31:16]=GPIO_149  - select mux mode 4 for gpio
	CONTROL_PADCONF_UART1_CTS &= 0xFFFF0000; //  [15:0] =GPIO_150  - Clear register [15:0]
	CONTROL_PADCONF_UART1_CTS |= 0x00000004; //  [15:0] =GPIO_150  - select mux mode 4 for gpio

	// Switch on the Interface and functional clocks to the GPIO5 module
	CM_FCLKEN_PER |= 0xFFFFFFF; // Enable GPIO5 F clock
	CM_ICLKEN_PER |= 0xFFFFFFF; // Enable GPIO5 I clock

	// Configure the GPIO signals
	GPIO5_OE &= ~(LED1+LED0); // Set GPIO_149 & GPIO_150 (GPIO 4 bit 2) to output
	GPIO5_OUT |= LED0; // Set GPIO_150 high
	GPIO5_OUT &= ~LED1; // Set GPIO_149 low

	while(1) {
		for(i=0; i<0x100000; i++) { }

		GPIO5_OUT ^= LED0;
		GPIO5_OUT ^= LED1;
	}
}

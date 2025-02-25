#include "stm32mp1xx.h"
#include "stm32mp1xx_ll_gpio.h"
#include "stm32mp1xx_ll_usart.h"
#include <stdint.h>

void delay_long();
void soft_breakpoint();
void write(const char *str);
static inline void delay_for_uart(void);
static inline void init_leds();

const char *global_constant_string = "And hi to you too!\r\n";

// LEDs:
// PA14, PA13, PH7, PD11

void main()
{
	// Test UART
	UART4->TDR = 'X';
	delay_for_uart();
	UART4->TDR = 'Y';
	delay_for_uart();
	UART4->TDR = 'Z';
	delay_for_uart();
	UART4->TDR = '\r';
	delay_for_uart();
	UART4->TDR = '\n';
	delay_for_uart();

	// Test LEDs
	init_leds();

	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_14);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_14);

	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_13);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_13);

	/*LL_GPIO_SetOutputPin(GPIOH, LL_GPIO_PIN_7);*/
	/*LL_GPIO_ResetOutputPin(GPIOH, LL_GPIO_PIN_7);*/

	LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_11);
	LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_11);

	// Test function calls (tests the stack)
	// So far, every function call has been inline
	const char *s = "Hello world from bare-metal!\r\n";
	write(s);

	// Test global static memory
	while (*global_constant_string != '\0') {
		UART4->TDR = *global_constant_string;
		global_constant_string++;
		delay_for_uart();
	}

	// Blink those lights forever!
	while (1) {
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_14);
		delay_long();
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_14);

		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_13);
		delay_long();
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_13);

		/*LL_GPIO_SetOutputPin(GPIOH, LL_GPIO_PIN_7);*/
		/*delay_long();*/
		/*LL_GPIO_ResetOutputPin(GPIOH, LL_GPIO_PIN_7);*/

		LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_11);
		delay_long();
		LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_11);
	};
}

void delay_long()
{
	uint32_t i = 0x1000000;
	while (i--)
		;
}

void write(const char *str)
{
	while (*str) {
		UART4->TDR = *str++;
		delay_for_uart();
	}
}

// Inline because we don't want to assume function calling and returning works yet!
static inline void delay_for_uart(void)
{
	while ((UART4->ISR & USART_ISR_TXFT) == 0)
		;
}

// Inline because we don't want to assume function calling and returning works yet!
static inline void init_leds()
{
	// OSD32MP1 board:
	// GPIO I, pin 8 = red D2
	// GPIO I, pin 9 = green D2
	// GPIO Z, pin 6 = red D1
	// GPIO Z, pin 7 = green D1
	// all LEDs are active low
    // STM32MP157F-DK2 LEDs:
    // PA14, PA13, PH7, PD11

	// Enable RCC for GPIO I and GPIO Z (for the LEDs on the OSD32 board)
	RCC->MC_AHB4ENSETR |= RCC_MC_AHB4ENSETR_GPIOAEN;
	/*RCC->MC_AHB4ENSETR |= RCC_MC_AHB4ENSETR_GPIOHEN;*/
	RCC->MC_AHB4ENSETR |= RCC_MC_AHB4ENSETR_GPIODEN;

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_14, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_MEDIUM);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_MEDIUM);
	/*LL_GPIO_SetPinMode(GPIOH, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);*/
	/*LL_GPIO_SetPinOutputType(GPIOH, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);*/
	/*LL_GPIO_SetPinSpeed(GPIOH, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_MEDIUM);*/
	LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_MEDIUM);
}

// Handy utility when using a debugger
void soft_breakpoint()
{
	volatile int stop = 1;
	while (stop) {
		// Attach a debugger and manually change the value at the address of `stop` in RAM from 1 to 0
	}
}

void IRQ_Initialize(void)
{
	// do nothing, just a stub
}

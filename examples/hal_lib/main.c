#include "main.h"
#include "stm32mp1xx_hal.h"

void UART4_Init(void);
void GPIO_Init(void);
void Error_Handler(void);
void soft_breakpoint();
void toggle_leds();

UART_HandleTypeDef huart4;
const char *global_constant_string = "And hi to you too!\r\n";

void main() {
	HAL_Init();
	HAL_Delay(1);
	GPIO_Init();
	UART4_Init();
	
    // disable buffering for the input stream
    // https://forum.digikey.com/t/easily-use-scanf-on-stm32/21103
    setvbuf(stdin, NULL, _IONBF, 0);

	printf("\r\nHello from HAL Driver!\r\n");
	printf("%s", global_constant_string);
    toggle_leds();

    int i;
    while(1){
        printf("Enter LEDs mask to toggle [0-7]:\r\n>>> ");
        scanf("%d", &i);
        if(i & 1){
            HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
        }
        if(i & 2){
            HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
        }
        if(i & 4){
            HAL_GPIO_TogglePin(LED_BLUE_PORT, LED_BLUE_PIN);
        }
        printf("\r\nDone.\r\n");
    }
}

void toggle_leds(){
    HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
    HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
    HAL_GPIO_TogglePin(LED_BLUE_PORT, LED_BLUE_PIN);
}

void UART4_Init(void)
{
	huart4.Instance = UART4;
	huart4.Init.BaudRate = 115200;
	huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart4.FifoMode = UART_FIFOMODE_ENABLE;
	huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if(HAL_UART_Init(&huart4) != HAL_OK) {
		Error_Handler();
	}
}

void GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitStruct.Pin = LED_RED_PIN | LED_GREEN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED_BLUE_PIN;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

PUTCHAR_PROTOTYPE {
  HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

GETCHAR_PROTOTYPE {
  uint8_t ch = 0;

  /* Clear the Overrun flag just before receiving the first character */
  __HAL_UART_CLEAR_OREFLAG(&huart4);

  /* Wait for reception of a character on the USART RX line and echo this
   * character on console */
  HAL_UART_Receive(&huart4, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

// Handy utility when using a debugger
void soft_breakpoint() {
	volatile int stop = 1;
	while (stop) {
		// Attach a debugger and manually change the value at the address of `stop` in RAM from 1 to 0
	}
}

void IRQ_Initialize(void) {
	// do nothing, just a stub
}

void Error_Handler() {
	while(1) {
		// handle error
	}
}

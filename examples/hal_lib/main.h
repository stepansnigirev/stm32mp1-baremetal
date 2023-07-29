#ifndef MAIN_H_
#define MAIN_H_

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

// LEDs:
// PA14, PA13, PH7, PD11
#define LED_GREEN_PORT GPIOA
#define LED_GREEN_PIN GPIO_PIN_14
#define LED_RED_PORT GPIOA
#define LED_RED_PIN GPIO_PIN_13
#define LED_BLUE_PORT GPIOD
#define LED_BLUE_PIN GPIO_PIN_11

#endif /* MAIN_H_ */

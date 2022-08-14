/**
 * Hello world for HD44780U LCD screen.
 */

#include "hd44780u_lcd.h"
#include "system_clock.h"
#include "user_button.h"

#include <stddef.h>

#define make_lcd_pin(gpio, num) { \
		.mode_reg = &GPIO ## gpio->MODER,			\
		.mode_reg_clear_mask = ~(GPIO_MODER_MODE ## num ## _0 | GPIO_MODER_MODE ## num ## _1), \
		.mode_reg_output_mask = GPIO_MODER_MODE ## num ## _0,	\
		.mode_reg_input_mask = 0,				\
		.output_data_reg = &GPIO ## gpio->ODR,			\
		.output_data_reg_mask = GPIO_ODR_OD ## num,		\
	}

struct hd44780u_lcd lcd = {
	.delay_microseconds_fn = systick_delay_microseconds,
	.rs_pin = make_lcd_pin(A, 6),
	.rw_pin = make_lcd_pin(A, 7),
	.e_pin = make_lcd_pin(B, 6),
	.data_pins = {
		make_lcd_pin(A, 9),
		make_lcd_pin(A, 8),
		make_lcd_pin(B, 10),
		make_lcd_pin(A, 10),
	},
};

char *messages[] = {
	"Hello, Amy!",
	"Bum diggity",
};

#define NUM_MESSAGES (sizeof messages / sizeof messages[0]);

volatile size_t message_index = 0;

void refresh_message(void)
{
	hd44780u_lcd_clear(&lcd);
	hd44780u_lcd_write_string(&lcd, messages[message_index]);
}

void start(void)
{
	// Enable GPIO ports
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	user_button_enable();
	systick_enable_passive();

	hd44780u_lcd_init(&lcd);
	refresh_message();
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR & (1 << USER_BUTTON_PIN)) {
		// Clear the EXTI status flag.
		EXTI->PR |= (1 << USER_BUTTON_PIN);

		// Pick next message
		message_index = (message_index + 1) % NUM_MESSAGES;
		refresh_message();
	}
}

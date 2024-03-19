/*
 * led.c
 *
 *  Created on: Mar 19, 2024
 *      Author: Admin
 */

#include "led.h"
#include "scheduler.h"
#include "main.h"
#include "debug.h"
static	led_state_t led_state;
static	uint32_t	volatile led_timer_count = 0;

#define		LED_DURATION	1000
uint32_t	is_led_timer_fired()
{
	uint32_t	_led_timer_count;
	__disable_irq();
	_led_timer_count = led_timer_count;
	__enable_irq();
	return	(_led_timer_count == 0) ? 1 : 0;
}

void	led_set_timer_ms(uint32_t	ms)
{
	__disable_irq();
	led_timer_count = ( ms * scheduler_get_tick()) / 1000 - 1;
	__enable_irq();
}
 void led_timer_systick_interrupt_handler()
 {
	 if (led_timer_count > 0)	led_timer_count--;
 }

 void	led_init()
 {
	 led_state = LED_OFF;
	 LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
 }
 void	led_task()
 {
	 switch (led_state)
	 {
	 	 case	LED_OFF:
	 		 if (is_led_timer_fired())
			 {
	 			LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
	 			led_set_timer_ms(LED_DURATION);
	 			led_state = LED_ON;
	 			DEBUG_PRINT("change to LED_ON\n");
			 }
	 		 break;
	 	 case	LED_ON:
	 		 if (is_led_timer_fired())
			 {
	 			LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
	 			led_set_timer_ms(LED_DURATION);
	 			led_state = LED_OFF;
	 			DEBUG_PRINT("change to LED_OFF\n");
			 }
	 		 break;
	 }


 }

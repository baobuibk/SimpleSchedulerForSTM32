/*
 * led.h
 *
 *  Created on: Mar 19, 2024
 *      Author: Admin
 */

#ifndef LED_H_
#define LED_H_
#include <stdbool.h>
#include <stdint.h>
typedef enum {LED_ON, LED_OFF}	led_state_t;
void	led_init();
void	led_task();
void led_timer_systick_interrupt_handler();
#endif /* LED_H_ */

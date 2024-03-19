/*
 * heater_control.c
 *
 *  Created on: Feb 27, 2024
 *      Author: Admin
 */
#include <heater.h>
#include "main.h"
#include "debug.h"
#include "scheduler.h"
static	heater_state_t	heater_state;

uint32_t	volatile heater_timer_count = 0;

uint32_t	heater_timer_fired()
{
	uint32_t	_heater_timer_count;
	__disable_irq();
	_heater_timer_count = heater_timer_count ;
	__enable_irq();
	return (_heater_timer_count == 0) ? 1 : 0;
}
void	heater_set_timer(uint32_t	ms)
{
	__disable_irq();
	heater_timer_count = (ms * scheduler_get_tick()) / 1000;
	__enable_irq();
}

void	heater_timer_systick_interrupt_handler()
{
	if (heater_timer_count != 0) heater_timer_count--;
}

bool	heater_is_button_pressed()
{
	if (LL_GPIO_IsInputPinSet(BUTTON_GPIO_Port, BUTTON_Pin))	return	true;
	else return false;
}

void heater_relay_control(bool state)
{
	if (state)	LL_GPIO_SetOutputPin(RELAY_GPIO_Port, RELAY_Pin);
	else LL_GPIO_ResetOutputPin(RELAY_GPIO_Port, RELAY_Pin);
}
void heater_init_state()
{
	heater_state = HEATER_OFF;
	heater_relay_control(false);
}
 void	heater_state_update()
 {
	 switch (heater_state)
	 {
	 case HEATER_OFF:
		 if (heater_is_button_pressed()	)
		 {
			 heater_state = HEATER_WAIT_ON;
			 heater_set_timer(2000);
			 DEBUG_PRINT("change to HEATER_WAIT_ON\n");
		 }
		 break;

	 case HEATER_WAIT_ON:
		 if (! heater_is_button_pressed())
		 {
			 heater_state = HEATER_OFF;
			 DEBUG_PRINT("change to HEATER_OFF\n");
		 }
		 else if (heater_timer_fired())
		 {
			 heater_state = HEATER_ON;
			 heater_relay_control(true);
			 DEBUG_PRINT("change to HEATER_ON\n");
		 }
		 break;
	 case HEATER_ON:
		 if (heater_is_button_pressed()	)
		 		 {
		 			 heater_state = HEATER_WAIT_OFF;
		 			 heater_set_timer(4000);
		 			DEBUG_PRINT("change to HEATER_WAIT_OFF\n");
		 		 }
		 		 break;
	 case HEATER_WAIT_OFF:
		 if (! heater_is_button_pressed())
				 {
					 heater_state = HEATER_ON;
					 DEBUG_PRINT("change to HEATER_ON\n");
				 }
				 else if (heater_timer_fired())
				 {
					 heater_state = HEATER_OFF;
					 heater_relay_control(false);
					 DEBUG_PRINT("change to HEATER_OFF\n");
				 }
				 break;
	 }
 }
 void	heater_task()
 {
	 heater_state_update();
 }

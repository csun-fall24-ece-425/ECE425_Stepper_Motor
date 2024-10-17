/*
 * @file main.c
 *
 * @brief Main source code for the Stepper_Motor_Example program.
 *
 * @author Aaron Nanas
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "PMOD_BTN_Interrupt.h"

// Declare the user-defined function prototype for PMOD_BTN_Interrupt
void PMOD_BTN_Handler(uint8_t pmod_btn_status);

// Declare the function prototype for the Stepper Motor initialization function
void Stepper_Motor_Init(void);

static const uint8_t half_step[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};

static uint8_t enable = 0;
static uint8_t direction_select = 0;

int main(void)
{
	// Initialize the SysTick timer used to provide blocking delay functions
	SysTick_Delay_Init();
	
	// Initialize the push buttons on the PMOD BTN module (Port A)
	PMOD_BTN_Interrupt_Init(&PMOD_BTN_Handler);
	
	// Initialize the pins used by the 28BYJ-48 Stepper Motor and ULN2003 Stepper Motor Driver
	Stepper_Motor_Init();
	
	int i = 0;
	
	while(1)
	{
		if (enable == 1)
		{
			for (int index = 0; index < 4100; index++)
			{
				if (enable == 0)
				{
					break;
				}
				
				GPIOB->DATA = (GPIOB->DATA & ~0x0F) | half_step[i];
				SysTick_Delay1us(1200);
				
				// Set the direction of the stepper motor to clockwise
				if (direction_select == 0)
				{
					i = (i + 1) % 8;
				}
				
				// Set the direction of the stepper motor to counter-clockwise
				else
				{
					i = (i - 1 + 8) % 8;
				}
			}
		}
		else
		{
			GPIOB->DATA &= ~0x0F;
			SysTick_Delay1us(1200);
		}
	}
}

/**
 * @brief
 *
 * @param uint8_t pmod_btn_status Indicates the status of the buttons on the PMOD BTN module.
 *
 * @return None
 */
void PMOD_BTN_Handler(uint8_t pmod_btn_status)
{
	switch(pmod_btn_status)
	{
		// PMOD BTN0 (PA2) is pressed
		case 0x04:
		{
			enable = 1;
			break;
		}
		
		// PMOD BTN1 (PA3) is pressed
		case 0x08:
		{
			enable = 0;
			break;
		}
		
		// PMOD BTN2 (PA4) is pressed
		case 0x10:
		{
			direction_select = direction_select + 1;
			direction_select = direction_select % 2;
			break;
		}
		
		// PMOD BTN3 (PA5) is pressed
		case 0x20:
		{

			break;
		}
		
		default:
		{
			break;
		}
	}
}

/**
 * @brief
 *
 * @param None
 *
 * @return None
 */
void Stepper_Motor_Init(void)
{
	// Enable the clock to GPIO Port B
	SYSCTL->RCGCGPIO |= 0x02;
	
	// Set PB0, PB1, PB2, and PB3 as output GPIO pins
	GPIOB->DIR |= 0x0F;
	
	// Configure PB0, PB1, PB2, and PB3 to function as GPIO pins
	GPIOB->AFSEL &= ~0x0F;
	
	// Enable Digital Functionality for PB0, PB1, PB2, and PB3
	GPIOB->DEN |= 0x0F;
	
	// Enable the clock to GPIO Port F
	SYSCTL->RCGCGPIO |= 0x20;
	
	// Set PF3 and PF2 as output GPIO pins
	GPIOF->DIR |= 0x0C;
	
	// Configure PF3 and PF2 to function as GPIO pins
	GPIOF->AFSEL &= ~0x0C;
	
	// Enable Digital Functionality for PF3 and PF2
	GPIOF->DEN |= 0x0C;
	
	// Initialize the output of PF3 and PF2 to high
	GPIOF->DATA |= 0x0C;
}

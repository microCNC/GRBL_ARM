/**************************************************************************//**
 * @file     system_TM4C.c
 * @brief    CMSIS Device System Source File for
 *           Texas Instruments TIVA TM4C123 Device Series
 * @version  V1.00
 * @date     27. March 2013
 *
 * @note
 *                                                             modified by Keil
 ******************************************************************************/

#include "system_TM4C123.h"
#include "TM4C123GH6PM.h"

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{

    uint32_t i;


  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */
  #endif

	// SET SYSTEM CLOCK TO 80MHZ
	
#define SYSDIV2 6
	
#define SYSCTL_RIS_PLLLRIS      0x00000040  // PLL Lock Raw Interrupt Status
#define SYSCTL_RCC_XTAL_M       0x000007C0  // Crystal Value
#define SYSCTL_RCC_XTAL_6MHZ    0x000002C0  // 6 MHz Crystal
#define SYSCTL_RCC_XTAL_8MHZ    0x00000380  // 8 MHz Crystal
#define SYSCTL_RCC_XTAL_16MHZ   0x00000540  // 16 MHz Crystal
#define SYSCTL_RCC2_USERCC2     0x80000000  // Use RCC2
#define SYSCTL_RCC2_DIV400      0x40000000  // Divide PLL as 400 MHz vs. 200
                                            // MHz
#define SYSCTL_RCC2_SYSDIV2_M   0x1F800000  // System Clock Divisor 2
#define SYSCTL_RCC2_SYSDIV2LSB  0x00400000  // Additional LSB for SYSDIV2
#define SYSCTL_RCC2_PWRDN2      0x00002000  // Power-Down PLL 2
#define SYSCTL_RCC2_BYPASS2     0x00000800  // PLL Bypass 2
#define SYSCTL_RCC2_OSCSRC2_M   0x00000070  // Oscillator Source 2
#define SYSCTL_RCC2_OSCSRC2_MO  0x00000000  // MOSC

// configure the system to get its clock from the PLL
  // 0) configure the system to use RCC2 for advanced features
  //    such as 400 MHz PLL and non-integer System Clock Divisor
  SYSCTL->RCC2 |= SYSCTL_RCC2_USERCC2;
	
  // 1) bypass PLL while initializing
  SYSCTL->RCC2 |= SYSCTL_RCC2_BYPASS2;
	
  // 2) select the crystal value and oscillator source
  SYSCTL->RCC &= ~SYSCTL_RCC_XTAL_M;													// clear XTAL field
  SYSCTL->RCC += SYSCTL_RCC_XTAL_16MHZ;												// configure for 16 MHz crystal
  SYSCTL->RCC2 &= ~SYSCTL_RCC2_OSCSRC2_M;											// clear oscillator source field
  SYSCTL->RCC2 += SYSCTL_RCC2_OSCSRC2_MO;											// configure for main oscillator source
	
  // 3) activate PLL by clearing PWRDN
  SYSCTL->RCC2 &= ~SYSCTL_RCC2_PWRDN2;
	
  // 4) set the desired system divider and the system divider least significant bit
  SYSCTL->RCC2 |= SYSCTL_RCC2_DIV400;													// use 400 MHz PLL
  SYSCTL->RCC2 = (SYSCTL->RCC2&~0x1FC00000) + (SYSDIV2<<22);	// clear system clock divider field configure for 80 MHz clock
	
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL->RIS&SYSCTL_RIS_PLLLRIS)==0){};
		
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL->RCC2 &= ~SYSCTL_RCC2_BYPASS2;
		
//
// SETUP SYSTICK
//
	SysTick->CTRL = 0;                   			// disable SysTick during setup
  SysTick->LOAD = SysTick_VAL_CURRENT_Msk;	// maximum reload value
  SysTick->VAL = 0;													// any write to current clears it
																						// enable SysTick with core clock
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk+SysTick_CTRL_CLKSOURCE_Msk;
}

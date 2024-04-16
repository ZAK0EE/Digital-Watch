#ifndef NVIC_H_
#define NVIC_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "stm32f401xx.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/* Define standard types for 8-bit unsigned and signed integers */
typedef unsigned char u8;
typedef signed char s8;

/* Define standard types for 16-bit unsigned and signed integers */
typedef unsigned short int u16;
typedef signed short int s16;

/* Define standard types for 32-bit unsigned and signed integers */
typedef unsigned int u32;
typedef signed int s32;

/* Define a NULL pointer constant */
#define NULLPTR ((void *)0)


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

void NVIC_EnableIRQ(IRQn_t IRQn);                  //Enable an external interrupt
void NVIC_DisableIRQ(IRQn_t IRQn);                 //Disable an external interrupt

void NVIC_SetPriority(IRQn_t IRQn, u32 priority);  //Set the priority of an interrupt
u32 NVIC_GetPriority(IRQn_t IRQn);                    //Read priority of IRQn

void __enable_irq(void);                               //Clear PRIMASK to enable interrupts
void __disable_irq(void);                              //Set PRIMASK to disable all interrupts

void NVIC_SetPriorityGrouping(u32 PriorityGroup);     //Set priority grouping configuration

void NVIC_SetPendingIRQ(IRQn_t IRQn);                 //Set IRQn pending
void NVIC_ClearPendingIRQ(IRQn_t IRQn);               //Clear IRQn pending status
u32 NVIC_GetPendingIRQ(IRQn_t IRQn);                  //Return true (IRQ-Number) if IRQn is pending

u32 NVIC_GetActive(IRQn_t IRQn);                      //Return the IRQ number of the active interrupt

void NVIC_SystemReset(void);                          //Reset the system

#endif 
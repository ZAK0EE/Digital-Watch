/*
 ============================================================================
 Name        : NVIC.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the NVIC Driver
 Date        : 3/3/2024
 ============================================================================
 */
#ifndef NVIC_H_
#define NVIC_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include  	"LIB/std_types.h"
#include 	"LIB/Mask32.h"
#include 	"LIB/Error.h"
#include	"LIB/Stm32F401cc.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PRIORITY_GROUP0		0x05FA0000 /*[4] bits Preempt Group &  [0] bits Subpriority Group*/
#define PRIORITY_GROUP1		0x05FA0400 /*[3] bits Preempt Group &  [1] bits Subpriority Group*/
#define PRIORITY_GROUP2		0x05FA0500 /*[2] bits Preempt Group &  [2] bits Subpriority Group*/
#define PRIORITY_GROUP3		0x05FA0600 /*[1] bits Preempt Group &  [3] bits Subpriority Group*/
#define PRIORITY_GROUP5		0x05FA0700 /*[0] bits Preempt Group &  [4] bits Subpriority Group*/
/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/*
 * @brief    : Enable NVIC IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Enables the specified NVIC interrupt.
 */
Error_enumStatus_t Enable_NVIC_IRQ(IRQn_t IRQn);

/*
 * @brief    : Disable NVIC IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Disables the specified NVIC interrupt.
 */
Error_enumStatus_t Disable_NVIC_IRQ(IRQn_t IRQn);

/*
 * @brief    : Set NVIC Pending IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Sets the specified NVIC interrupt as pending.
 */
Error_enumStatus_t Set_NVIC_Pending_IRQ(IRQn_t IRQn);

/*
 * @brief    : Clear NVIC Pending IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Clears the pending status of the specified NVIC interrupt.
 */
Error_enumStatus_t Clear_NVIC_Pending_IRQ(IRQn_t IRQn);

/*
 * @brief    : Get NVIC Pending IRQ status
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Ptr_u8Status: Pointer to store the status
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Retrieves the pending status of the specified NVIC interrupt.
 */
Error_enumStatus_t Get_NVIC_Pending_IRQ(IRQn_t IRQn, uint8_t *Ptr_u8Status);

/*
 * @brief    : Get NVIC Active IRQ status
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Ptr_u8Status: Pointer to store the status
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Retrieves the active status of the specified NVIC interrupt.
 */
Error_enumStatus_t Get_NVIC_Active_IRQ(IRQn_t IRQn, uint8_t *Ptr_u8Status);

/*
 * @brief    : Set Interrupt Priority
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Copy_PreemptGroup: Preemption priority group
 * @param[in]: Copy_SubpriorityGroup: Subpriority group
 * @param[in]: GroupPriority: Group priority value
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Sets the priority level for the specified NVIC interrupt.
 */
Error_enumStatus_t Set_Interrupt_Priority(IRQn_t IRQn, uint8_t Copy_PreemptGroup ,uint8_t Copy_SubpriorityGroup ,uint32_t GroupPriority );

/*
 * @brief    : Get Interrupt Priority
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Ptr_u8Status: Pointer to store the priority level
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Retrieves the priority level of the specified NVIC interrupt.
 */
Error_enumStatus_t Get_Interrupt_Priority(IRQn_t IRQn, uint8_t *Ptr_u8Status);

/*
 * @brief    : Generated a Software  Interrupt
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : The value to be written is
			   the Interrupt ID of the required SGI, in the range 0-239. 
			   For example, a value of 0x03 specifies interrupt IRQ3.
 */
Error_enumStatus_t SET_Software_Interrupt(IRQn_t IRQn);
#endif /* NVIC_H_ */








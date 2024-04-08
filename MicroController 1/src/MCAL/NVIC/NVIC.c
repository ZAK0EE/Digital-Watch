/*
 ============================================================================
 Name        : NVIC.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the NVIC Driver
 Date        : 3/3/2024
 ============================================================================
 */

/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
/* Include the header file for NVIC driver */
#include "MCAL/NVIC/NVIC.h"
/*******************************************************************************
 *                             Definitions                                      *
 *******************************************************************************/
#define NVIC_BASE_ADDRESS            0xE000E100 /* Base address of NVIC peripheral */
#define SCB_BASE_ADDRESS             0xE000ED00 /* Base address of SCB peripheral */
#define GROUP_SHIFT_MASK             0x05FA0300 /* Mask for priority group */
#define BITS_PER_GROUP               32 /* Number of bits per priority group */
#define MAX_ACTIVE_PROPRITY_BITS     15 /* Maximum number of active priority bits */
#define NUM_INT_IN_IPR_X             4 /* Number of interrupts per IPR register */
#define NUM_BITS_PER_EACH_IPR        8 /* Number of interrupts per IPR register */
#define NUM_BITS_TO_CONTROL_GROUPING 4
#define SHIFTING_DIVISION_FACTOR     256
#define PREV_VALUE_CLR_MASK          0x0000000F
/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef struct
{

    /* Interrupt set-enable register  */
    uint32_t NVIC_ISER[8];
    /* Reserved Bits */
    uint32_t RESERVED1[24];
    /*Interrupt clear-enable register   */
    uint32_t NVIC_ICER[8];
    /* Reserved Bits */
    uint32_t RESERVED2[24];
    /* Interrupt set-pending register */
    uint32_t NVIC_ISPR[8];
    /* Reserved Bits */
    uint32_t RESERVED3[24];
    /* Interrupt clear-pending register  */
    uint32_t NVIC_ICPR[8];
    /* Reserved Bits */
    uint32_t RESERVED4[24];
    /* Interrupt active bit register  */
    uint32_t NVIC_IABR[8];
    /* Reserved Bits */
    uint32_t RESERVED5[56];
    /* Interrupt priority register  */
    uint32_t NVIC_IPR[60];
    /* Reserved Bits */
    uint32_t RESERVED6[644];
    /* Software trigger interrupt register */
    uint32_t NVIC_STIR;
} NVIC_PERI_t;

typedef struct
{
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHPR1;
    uint32_t SHPR2;
    uint32_t SHPR3;
    uint32_t SHCSR;
    uint32_t CFSR;
    uint32_t HFSR;
    uint32_t reserved;
    uint32_t MMAR;
    uint32_t BFAR;
    uint32_t AFSR;
} SCB_PERI_t;
/*******************************************************************************
 *                              Variables		                                *
 *******************************************************************************/
/* Pointer to NVIC peripheral */
volatile NVIC_PERI_t *const NVIC = (volatile NVIC_PERI_t *)NVIC_BASE_ADDRESS;
/* Pointer to SCB peripheral */
volatile SCB_PERI_t *const SCB = (volatile SCB_PERI_t *)SCB_BASE_ADDRESS;

/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*
 * @brief    : Enable NVIC IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Enables the specified NVIC interrupt.
 */
Error_enumStatus_t Enable_NVIC_IRQ(IRQn_t IRQn)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / BITS_PER_GROUP;
    /* If IRQn is out of range, set error status */
    if (IRQn >= _INT_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Enable the specified NVIC interrupt */
        NVIC->NVIC_ISER[Loc_u8Index] = (1 << (IRQn % BITS_PER_GROUP));
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Disable NVIC IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Disables the specified NVIC interrupt.
 */
Error_enumStatus_t Disable_NVIC_IRQ(IRQn_t IRQn)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / BITS_PER_GROUP;
     /* Local Variable to store error status */
    if (IRQn >= _INT_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Disable the specified NVIC interrupt */
        NVIC->NVIC_ICER[Loc_u8Index] = (1 << (IRQn % BITS_PER_GROUP));
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Set NVIC Pending IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Sets the specified NVIC interrupt as pending.
 */
Error_enumStatus_t Set_NVIC_Pending_IRQ(IRQn_t IRQn)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / BITS_PER_GROUP;
    if (IRQn >= _INT_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Set the specified NVIC interrupt as pending */
        NVIC->NVIC_ISPR[Loc_u8Index] = (1 << (IRQn % BITS_PER_GROUP));
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Clear NVIC Pending IRQ
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Clears the pending status of the specified NVIC interrupt.
 */
Error_enumStatus_t Clear_NVIC_Pending_IRQ(IRQn_t IRQn)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / BITS_PER_GROUP;
    if (IRQn >= _INT_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Clear the pending status of the specified NVIC interrupt */
        NVIC->NVIC_ICPR[Loc_u8Index] = (1 << (IRQn % BITS_PER_GROUP));
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Get NVIC Pending IRQ status
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Ptr_u8Status: Pointer to store the status
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Retrieves the pending status of the specified NVIC interrupt.
 */
Error_enumStatus_t Get_NVIC_Pending_IRQ(IRQn_t IRQn, uint8_t *Ptr_u8Status)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / BITS_PER_GROUP;
    /* If IRQn is out of range, set error status */
    if (IRQn >= _INT_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else if (Ptr_u8Status == NULL)
    {
        /* If Ptr_u8Status is a NULL Pointer, set error status */
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        /* Retrieve the pending status of the specified NVIC interrupt */
        *Ptr_u8Status = ((NVIC->NVIC_ISPR[Loc_u8Index]) >> (IRQn % BITS_PER_GROUP)) & 0x01;
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Get NVIC Active IRQ status
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Ptr_u8Status: Pointer to store the status
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Retrieves the active status of the specified NVIC interrupt.
 */
Error_enumStatus_t Get_NVIC_Active_IRQ(IRQn_t IRQn, uint8_t *Ptr_u8Status)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / BITS_PER_GROUP;
    /* If IRQn is out of range, set error status */
    if (IRQn >= _INT_Num)
    {
    /* If IRQn is out of range, set error status */   
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else if (Ptr_u8Status == NULL)
    {
        /* If Ptr_u8Status is NULL, set error status */
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        /* Retrieve the active status of the specified NVIC interrupt */
       *Ptr_u8Status = ((NVIC->NVIC_IABR[Loc_u8Index]) >> (IRQn % BITS_PER_GROUP)) & 0x01;
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Set Interrupt Priority
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Copy_PreemptGroup: Preemption priority group
 * @param[in]: Copy_SubpriorityGroup: Subpriority group
 * @param[in]: GroupPriority: Group priority value
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Sets the priority level for the specified NVIC interrupt.
 */
Error_enumStatus_t Set_Interrupt_Priority(IRQn_t IRQn, uint8_t Copy_PreemptGroup, uint8_t Copy_SubpriorityGroup, uint32_t GroupPriority)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / NUM_INT_IN_IPR_X;
    /*Local Variable to hold the desired value of the 4 bits that will represent Preempt Group Subpriori tyGroup 
     * At first, I calulate the desired shifting for Preempt Group if it was 1,2,3,4 
     * Then , Put Subpriority Group Value 
     * At the end make bitwise or between Copy_SubpriorityGroup and Copy_PreemptGroup to adjust the 4 bits with desired values
     * If the user choose PRIORITY_GROUP0 so the value will  by Copy_PreemptGroup directly 
     */
    uint32_t Loc_ValueAssiged = (GroupPriority == PRIORITY_GROUP0) ? Copy_PreemptGroup : (Copy_SubpriorityGroup | (Copy_PreemptGroup << ((GroupPriority - GROUP_SHIFT_MASK) / SHIFTING_DIVISION_FACTOR)));
    /* Calculate the index of the required register according to IRQn for IPR resgter */
    uint8_t Loc_Shift_value = ((IRQn % NUM_INT_IN_IPR_X) * NUM_BITS_PER_EACH_IPR) + NUM_BITS_TO_CONTROL_GROUPING;
    /*Assign the current value of NVIC_IPR register in temp variable*/
    uint32_t Loc_TempReg = NVIC->NVIC_IPR[Loc_u8Index] ;
    /* If IRQn is out of range, set error status */
    if (Copy_PreemptGroup > MAX_ACTIVE_PROPRITY_BITS     ||
        Copy_SubpriorityGroup > MAX_ACTIVE_PROPRITY_BITS ||
        IRQn > _INT_Num                                  ||
        (GroupPriority < PRIORITY_GROUP0)                ||
        (GroupPriority > PRIORITY_GROUP5)                )
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {

        /*Prebare the last shape of desired data of grouping and assign in in temp variable */
        Loc_ValueAssiged = Loc_ValueAssiged << Loc_Shift_value ;
        /*Clear the previous value for the required register  */
        Loc_TempReg &= ~(PREV_VALUE_CLR_MASK <<Loc_Shift_value );
         /*Assign the new grouping data in Temp variable */
        Loc_TempReg |= Loc_ValueAssiged ;
        /*Assign the new grouping data in NVIC_IPR register*/
        NVIC->NVIC_IPR[Loc_u8Index] = Loc_TempReg ;
        /*Configure the 4bits of grouping shape according to GroupPriority*/
        SCB->AIRCR = GroupPriority ;
    }
    /*ٌReturn error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Get Interrupt Priority
 * @param[in]: IRQn: Interrupt number
 * @param[in]: Ptr_u8Status: Pointer to store the priority level
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : Retrieves the priority level of the specified NVIC interrupt.
 */
Error_enumStatus_t Get_Interrupt_Priority(IRQn_t IRQn, uint8_t *Ptr_u8Status)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Calculate the index of the required register according to IRQn */
    uint8_t Loc_u8Index = IRQn / NUM_INT_IN_IPR_X;
    /* Calculate the index of the required register according to IRQn for IPR resgter */
    uint8_t Loc_Shift_value = ((IRQn % NUM_INT_IN_IPR_X) * NUM_BITS_PER_EACH_IPR) + NUM_BITS_TO_CONTROL_GROUPING;
    /* If IRQn is out of range, set error status */
    if ( IRQn > _INT_Num     )           
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    /* If IRQn is out of range, set error status */ 
    else if ( Ptr_u8Status == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        /* Get the priority level of the specified NVIC interrupt*/
      * Ptr_u8Status = (( NVIC->NVIC_IPR[Loc_u8Index]) >> Loc_Shift_value ) & 0x0F ;
    }
    return Loc_enumReturnStatus;
}


/*
 * @brief    : Generated a Software  Interrupt
 * @param[in]: IRQn: Interrupt number
 * @return   : Error_enumStatus_t: Status of the operation
 * @details  : The value to be written is
               the Interrupt ID of the required SGI, in the range 0-239.
               For example, a value of 0x03 specifies interrupt IRQ3.
 */
Error_enumStatus_t SET_Software_Interrupt(IRQn_t IRQn)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* If IRQn is out of range, set error status */
    if (IRQn >= _INT_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Generated a Software Interrupt */
        NVIC->NVIC_STIR = IRQn;
    }
    return Loc_enumReturnStatus;
}

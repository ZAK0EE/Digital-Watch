/*
 ============================================================================
 Name        : CLK_CONTROL.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the CLK_CONTROL Module
 Date        : 5/3/2024
 ============================================================================
 */

/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "HAL\ControlClock\CLK_Control.h"
#include "MCAL\RCC\RCC.h"
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*
 * @brief    : Set Clock ON For Certain GPIO Port.
 * @param[in]: Copy_PortName Port Name.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source for specific GPIO portt.
 */
Error_enumStatus_t Set_Clock_ON(uint32_t Copy_PortName)
{
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint32_t Loc_u32BusID = Copy_PortName >> 29;
    switch (Loc_u32BusID)
    {
    case 0:
        RCC_Enable_AHB1_Peripheral(Copy_PortName);
        break;
    case 1:
        RCC_Enable_AHB2_Peripheral(Copy_PortName);
        break;
    case 2:
        RCC_Enable_APB1_Peripheral(Copy_PortName);
        break;
    case 3:
        RCC_Enable_APB2_Peripheral(Copy_PortName);
        break;
    default:
        break;
    }
    return Loc_enumReturnStatus;
}

/*
 ============================================================================
 Name        : HSwitch.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the HSwitch Driver (Solve Debounce problem)
 Date        : 15/3/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "HAL/HSwitch/HSwitch.h"
#include "MCAL/GPIO/GPIO.h"
/*******************************************************************************
 *                             Definitions                                      *
 *******************************************************************************/
#define Get_CURRENT_CONNECTION_SHIFT_MASK 3

/*******************************************************************************
 *                              Variables                                       *
 *******************************************************************************/
extern const HSwitch_Config_t HSwitchS[_HSwitch_Num];
static uint8_t HSwitch_Status[_HSwitch_Num];
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*
 * @brief    : Initializes a HSwitch pin based on the provided configuration.
 * @param[in]: Void.
 * @return   : Error_enumStatus_t - Error status indicating success or failure of the initialization.
 * @details  : This function initializes a HSwitch pin according to the provided configuration.
 */
Error_enumStatus_t HSwitch_Init(void)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /*Creat an object from  GPIO_Config_t sturct to configure the GPIO
      Peripheral according to user configuration for privided HSwitchs*/
    GPIO_PinConfig_t Loc_HSwitch_GPIO_Handler;
    /*Initiate a local index to loop with it */
    uint8_t Loc_idx = 0;
    /*Loop for each HSwitch to configure it's port and pin in GPIO*/
    for (Loc_idx = 0; Loc_idx < _HSwitch_Num; Loc_idx++)
    {
        /*Send the Pin of HSwitch number Loc_idx to configuartion structure in gpio*/
        Loc_HSwitch_GPIO_Handler.PinNumber = HSwitchS[Loc_idx].Pin;
        /*Send the Port of HSwitch number Loc_idx to configuartion structure in gpio*/
        Loc_HSwitch_GPIO_Handler.Port = HSwitchS[Loc_idx].Port;
        /*Send the Mood of HSwitch number Loc_idx to configuartion structure in gpio*/
        Loc_HSwitch_GPIO_Handler.PinMode = HSwitchS[Loc_idx].Connection;
        /*Init GPIO pins with the required configuration */
        Loc_enumReturnStatus = GPIO_initPin(&Loc_HSwitch_GPIO_Handler);
    }
    /*Return the error status*/
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Sets the value of a HSwitch pin to a specified state.
 * @param[in]: Copy_HSwitchName - HSwitch Name or ID.
 * @param[in]: Copy_HSwitchStatus - The state that desired to be set if it was HSwitch_ON or HSwitch_OFF .
 * @return   : Error_enumStatus_t - Error status indicating success or failure of setting the pin value.
 * @details  : This function sets the value of a HSwitch pin to the specified state (high or low).
 */
Error_enumStatus_t HSwitch_Get_Status(uint32_t Copy_HSwitchName, uint8_t *Ptr_Status)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /*Check if the HSwitch name is valid or not */
    if (Copy_HSwitchName >= _HSwitch_Num)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else if (Ptr_Status == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        /*Assign Switch value according to current value*/
        *Ptr_Status = HSwitch_Status[Copy_HSwitchName];
    }
    /*Return the error status*/
    return Loc_enumReturnStatus;
}

/* 
 * @brief    : Defines a function to handle switch debouncing and update switch status.
 * @param[in]: void .
 * @return   : Error_enumStatus_t - Error status indicating success or failure of setting the pin value.
 * @details  : This function to handle swirch debounce problem according to schedular .
 */
void HSwitch_Runnable(void)
{
    /* Declare variable to hold current switch state */
    uint8_t current;
    /* Declare static array to hold previous switch states */
    static uint8_t prev[_HSwitch_Num];
    /* Declare static array to hold switch state change counts */
    static uint32_t counts[_HSwitch_Num];
    /* Declare variable for loop index */
    uint8_t idx;
    /* Loop through all switches */
    for (idx = 0; idx < _HSwitch_Num; idx++)
    {
        /* Get current state of the switch */
        current = GPIO_getPinValue(HSwitchS[idx].Port, HSwitchS[idx].Pin);
        /* Check if current state is same as previous state */
        if (current == prev[idx])
        {
            /* Increment switch state change count */
            counts[idx]++;
        }
        else
        {
            /* Reset switch state change count */
            /*Notice that we can't remove this condition for some corner cases
             *Ex : (101) after first (1) handling for next step we will find prev = current which is wrong
             *So we will increment counter which is wrong as the was a zero come between the two ones
             */
             counts[idx] =0 ;
        }
        /* Check if switch state change count is multiple of 5 */
        if (counts[idx] % 5 == 0)
        {
            HSwitch_Status[idx] = current ^ (((HSwitchS[idx].Connection) >> Get_CURRENT_CONNECTION_SHIFT_MASK) & 0x1);
        }
        /* Update previous state with current state */
        prev[idx] = current;
    }
}
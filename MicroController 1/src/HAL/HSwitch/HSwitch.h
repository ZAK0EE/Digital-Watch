/*
 ============================================================================
 Name        : HSwitch.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the HSwitch Driver (Solve Debounce problem)
 Date        : 15/3/2024
 ============================================================================
 */
#ifndef HSwitch_H_
#define HSwitch_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include  	"LIB/std_types.h"
#include 	"LIB/Mask32.h"
#include 	"LIB/Error.h"
#include   	"HAL/HSwitch/HSwitch_Cfg.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define HSwitch_PRESSED 			0x00000001
#define HSwitch_RELEASED 			0x00000000
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/*Struct for new HHSwitch  configuration */
typedef struct
{
	uint32_t  Pin;
	void* 	  Port;
	uint32_t  Connection;
}HSwitch_Config_t;

/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/*
 * @brief    : Initializes a HSwitch pin based on the provided configuration.
 * @param[in]: Void.
 * @return   : Error_enumStatus_t - Error status indicating success or failure of the initialization.
 * @details  : This function initializes a HSwitch pin according to the provided configuration.
 */
Error_enumStatus_t HSwitch_Init (void);
/*
 * @brief    : Sets the value of a HSwitch pin to a specified state.
 * @param[in]: Copy_HSwitchName - HSwitch Name or ID.
 * @param[in]: Copy_HSwitchStatus - The state that desired to be set if it was HSwitch_PRESSED or HSwitch_RELEASED .
 * @return   : Error_enumStatus_t - Error status indicating success or failure of setting the pin value.
 * @details  : This function sets the value of a HSwitch pin to the specified state (high or low).
 */
Error_enumStatus_t HSwitch_Get_Status(uint32_t Copy_HSwitchName , uint8_t* Ptr_Status );




#endif /* HSwitch_H_ */

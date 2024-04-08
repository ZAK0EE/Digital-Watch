 /******************************************************************************
 *
 *
 * File Name: Error.h
 *
 *
 * Author: Omar Medhat
 *
 *******************************************************************************/

#ifndef ERROR_H_
#define ERROR_H_
typedef enum
{
	/**
	 *@brief : Everything Ok, Function had Performed Correctly.
	 */
	Status_enumOk ,
	/**
	 *@brief :NOT oK ERROR
	 */
	Status_enumNotOk ,
	/**
	 *@brief :Wrong Input error
	 */
	Status_enumWrongInput,
	/**
	 *@brief :Wrong Input error
	 */
	Status_enumNULLPointer,

	Status_enumBusyState,

	Status_enumTimOut

}Error_enumStatus_t;

#endif /* ERROR */


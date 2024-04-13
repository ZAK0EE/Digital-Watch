/*
 ============================================================================
 Name        : HUART.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the HUART Driver
 Date        : 12/4/2024
 ============================================================================
 */
#ifndef HUART_H_
#define HUART_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include  	"LIB/std_types.h"
#include 	"LIB/Mask32.h"
#include 	"LIB/Error.h"
#include   	"MCAL/UART/USART.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define HUART_DISPLAY_LINE1		0
#define HUSART1_ID 				0
#define HUSART2_ID 				1
#define HUSART6_ID 				2
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef void (*Cb)(void);

typedef struct
{
	uint8_t USART_ID;
	uint8_t *Ptr_buffer ;
	uint32_t Buff_Len ;
	Cb 		Buff_cb	;
}
HUSART_UserReq_t;
typedef struct
{
	uint8_t USART_ID;
	uint8_t TX_PORT;
	uint8_t TX_PIN;
	uint8_t TX_AF_ID ;
	uint8_t RX_PORT;
	uint8_t RX_PIN;
	uint8_t RX_AF_ID ;
}
HUSART_PINConfig_t;
/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/**
 * @brief    : Initializes the UART peripherals and associated GPIO pins.
 * @details  : This function initializes the UART peripherals and configures their associated GPIO pins for communication.
 *             It also enables the necessary NVIC interrupts for UART communication.
 *             - Configures GPIO pins for UART TX and RX.
 *             - Configures GPIO alternate function for UART TX and RX.
 *             - Enables NVIC interrupts for UART communication.
 *             - Initializes the UART peripherals.
 * @param    : None
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the UART initialization.
 **/
Error_enumStatus_t HUART_Init (void);

/**
 * @brief    : Initiates an asynchronous send operation for UART communication.
 * @details  : This function initiates an asynchronous send operation for UART communication.
 *             - Checks if the pointer to the UART send request structure is valid.
 *             - Determines the index of the UART channel based on the provided USART ID.
 *             - Checks if the UART send request state is ready to send data.
 *             - Updates the send request structure with the provided buffer information.
 *             - Sets the UART type to send mode.
 * @param[in]: Ptr_HUARTSendReq Pointer to the UART send request structure.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the send operation initiation.
 **/
Error_enumStatus_t HUART_SendBuffAsync(HUSART_UserReq_t* Ptr_HUARTSendReq);


/**
 * @brief    : Initiates an asynchronous receive operation for UART communication.
 * @details  : This function initiates an asynchronous receive operation for UART communication.
 *             - Checks if the pointer to the UART receive request structure is valid.
 *             - Checks if the UART receive request state is ready to receive data.
 *             - Updates the receive request structure with the provided buffer information.
 *             - Sets the UART type to receive mode.
 * @param[in]: Ptr_HUARTGetReq Pointer to the UART receive request structure.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the receive operation initiation.
 **/
Error_enumStatus_t HUART_ReceiveBuffAsync(HUSART_UserReq_t* Ptr_HUARTGetReq);
#endif
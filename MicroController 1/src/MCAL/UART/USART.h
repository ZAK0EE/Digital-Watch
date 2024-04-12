/*
 ============================================================================
 Name        : USART.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the USART Driver
 Date        : 30/3/2024
 ============================================================================
 */
#ifndef USART_H_
#define USART_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "LIB/std_types.h"
#include "LIB/Mask32.h"
#include "LIB/Error.h"
#include  "MCAL/UART//USART_Cfg.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define USART1_ID 				0
#define USART2_ID 				1
#define USART6_ID 				2
#define USART_WL_8BIT_DATA		0X00000000
#define USART_WL_9BIT_DATA		0X00001000
#define USART_PARITY_ENABLE		0X00000400
#define USART_PARITY_DISABLE	0X00000000
#define USART_EVEN_PARITY		0X00000000
#define USART_ODD_PARITY		0X00000200
#define USART_STOP_BIT_1		0X00000000
#define USART_STOP_BIT_1_5		0X00003000
#define USART_STOP_BIT_0_5		0X00001000
#define USART_STOP_BIT_2		0X00002000
#define USART_OVS_8				0X00008000
#define USART_OVS_16			0X00000000
#define FCPU					16000000
#define	Done					1
#define	NOT_Done				0
/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef void (*Cb)(void);
/**
 * @brief    : USART configuration structure.
 **/
typedef struct
{
	uint8_t 	USART_ID;
	uint32_t 	BaudRate;
	uint32_t 	WordLength;
	uint32_t 	ParityEn;
	uint32_t 	ParityType;
	uint32_t 	StopBits;
	uint32_t 	OverSamplingMode;
}
USART_Config_t;
/**
 * @brief    : USART user request structure.
 **/
typedef struct
{
	uint8_t USART_ID;
	uint8_t *Ptr_buffer ;
	uint32_t Buff_Len ;
	Cb 		Buff_cb	;
}
USART_UserReq_t;
/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/**
 * @brief    : Initializes USART communication.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of USART initialization.
 * @details  : This function initializes USART communication by configuring the USART peripheral,
 *             setting the baud rate, configuring frame format (word length, parity, stop bits),
 *             and enabling USART communication.
 */ 
Error_enumStatus_t USART_Init(void);
/**
 * @brief    : Transmits a single byte over USART.
 * @param[in]: Ptr_UserReq Pointer to USART user request structure containing transmit parameters.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the transmission.
 * @details  : This function transmits a single byte of data over USART.
 *             It checks for NULL pointer input and the length of the transmit buffer.
 *             If the USART transmit request is ready, it sets the request state to busy,
 *             transmits the byte of data, and waits for the transmission to complete.
 **/
Error_enumStatus_t USART_SendByte(USART_UserReq_t* Ptr_UserReq);
/**
 * @brief    : Receives a single byte over USART.
 * @param[in]: Ptr_UserReq Pointer to USART user request structure containing receive parameters.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the reception.
 * @details  : This function receives a single byte of data over USART.
 *             It checks for NULL pointer input and the length of the receive buffer.
 *             If the USART receive request is ready, it sets the request state to busy,
 *             enables USART receive, waits for a byte of data to be received, and reads the received byte.
 **/
Error_enumStatus_t USART_GetByte(USART_UserReq_t* Ptr_UserReq);
/**
 * @brief    : Asynchronously transmits data over USART.
 * @param[in]: Ptr_UserReq Pointer to USART user request structure containing transmit parameters.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the transmission.
 * @details  : This function initiates asynchronous transmission of data over USART.
 *             It checks for NULL pointer input and the state of the USART transmit request.
 *             If the USART transmit request is ready, it sets the request state to busy,
 *             copies the transmit buffer parameters from the user request structure,
 *             enables USART transmit, loads the first byte of data into the USART data register,
 *             and enables USART transmit data register empty interrupt.
 **/
Error_enumStatus_t USART_TxBufferAsyncZeroCopy(USART_UserReq_t* Ptr_UserReq );
/**
 * @brief    : Asynchronously receives data over USART.
 * @param[in]: Ptr_UserReq Pointer to USART user request structure containing receive parameters.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the reception.
 * @details  : This function initiates asynchronous reception of data over USART.
 *             It checks for NULL pointer input and the state of the USART receive request.
 *             If the USART receive request is ready, it clears RXNE flag,
 *             sets the request state to busy, copies the receive buffer parameters from the user request structure,
 *             enables USART receive, and enables USART receive data register not empty interrupt.
 **/
Error_enumStatus_t USART_RxBufferAsyncZeroCopy(USART_UserReq_t* Ptr_UserReq );
/**
 * @brief    : Checks if USART transmission is completed.
 * @param[in]: USART_ID   USART ID.
 * @param[out]: Ptr_Status Pointer to a variable to store the status of transmission completion.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the operation.
 * @details  : This function checks if the transmission over the specified USART is completed.
 *             It reads the USART status register to determine the transmission status.
 **/
Error_enumStatus_t USART_TxDone(uint8_t USART_ID,uint8_t *Ptr_Status);
/**
 * @brief    : Checks if USART is ready to receive data.
 * @param[in]: USART_ID   USART ID.
 * @param[out]: Ptr_Status Pointer to a variable to store the status of USART reception.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the operation.
 * @details  : This function checks if the specified USART is ready to receive data.
 *             It reads the USART status register to determine the reception status.
 **/
Error_enumStatus_t USART_IsRx(uint8_t USART_ID,uint8_t *Ptr_Status);


#endif
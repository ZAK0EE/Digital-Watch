/*
 ============================================================================
 Name        : USART_Cfg.c
 Author      : Omar Medhat Mohamed
 Description : Source Configuration file for the USART driver
 Date        : 30/3/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/UART/USART_Cfg.h"
#include "MCAL/UART/USART.h"

/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*Global array to set USARTs configuration*/
const  USART_Config_t USARTS[_USART_Num] =
{
 [UASART_1]={
    .USART_ID=USART1_ID,
    .BaudRate=9600,
    .WordLength=USART_WL_8BIT_DATA,
    .ParityEn=USART_PARITY_DISABLE,
    .ParityType=0,
    .StopBits=USART_STOP_BIT_1,
    .OverSamplingMode=USART_OVS_16}
};

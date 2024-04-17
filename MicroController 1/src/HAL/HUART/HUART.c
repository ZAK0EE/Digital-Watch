/*
 ============================================================================
 Name        : HUART.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the HUART Driver
 Date        : 12/4/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "HAL/HUART/HUART.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/NVIC/NVIC.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define UART_PINS_NUM 2
#define TX_ID 0
#define RX_ID 1

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef struct
{
    USART_UserReq_t BuffReqInfo;
} HUSART_SendReq_t;
typedef struct
{
    USART_UserReq_t BuffReqInfo;
} HUSART_GetReq_t;
/*******************************************************************************
 *                              Variables                                       *
 *******************************************************************************/
HUSART_SendReq_t SendReq[_USART_Num];
HUSART_GetReq_t GetReq[_USART_Num];
extern const HUSART_PINConfig_t HUARTS[_USART_Num];
extern uint8_t g_UART1_idx;
extern uint8_t g_UART2_idx;
extern uint8_t g_UART6_idx;
static uint8_t g_Index_Of_Sending;
static uint8_t g_Index_Of_Receiving;
/*******************************************************************************
 *                         Static Function Prototypes		                   *
 *******************************************************************************/

/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/

/****************************Processes Implementation************************************/
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
Error_enumStatus_t HUART_Init(void)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_idx;
    GPIO_PinConfig_t UART_PINS[UART_PINS_NUM] = {
        [TX_ID] = {.PinMode = GPIO_MODE_ALTERNATE_PUSHPULL_NOPULL, .PinSpeed = GPIO_SPEED_HIGH},
        [RX_ID] = {.PinMode = GPIO_MODE_ALTERNATE_PUSHPULL_NOPULL, .PinSpeed = GPIO_SPEED_HIGH}};

    /* Initialize GPIO pins for UART TX and RX */
    for (Loc_idx = 0; Loc_idx < _USART_Num; Loc_idx++)
    {
        UART_PINS[TX_ID].Port = HUARTS[Loc_idx].TX_PORT;
        UART_PINS[TX_ID].PinNumber = HUARTS[Loc_idx].TX_PIN;
        UART_PINS[RX_ID].Port = HUARTS[Loc_idx].RX_PORT;
        UART_PINS[RX_ID].PinNumber = HUARTS[Loc_idx].RX_PIN;

        GPIO_initPin(&UART_PINS[TX_ID]);
        GPIO_initPin(&UART_PINS[RX_ID]);

        /* Configure GPIO alternate function for UART TX and RX */
        GPIO_setPinAF(UART_PINS[TX_ID].Port, UART_PINS[TX_ID].PinNumber, HUARTS[Loc_idx].TX_AF_ID);
        GPIO_setPinAF(UART_PINS[RX_ID].Port, UART_PINS[RX_ID].PinNumber, HUARTS[Loc_idx].TX_AF_ID);
        /* Enable NVIC interrupts for UART communication */
        switch (HUARTS[Loc_idx].USART_ID)
        {
        case HUSART1_ID:
            Enable_NVIC_IRQ(USART1_IRQ);
            break;
        case HUSART2_ID:
            Enable_NVIC_IRQ(USART2_IRQ);
            break;
        case HUSART6_ID:
            Enable_NVIC_IRQ(USART6_IRQ);
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
    }

    /* Initialize the UART peripherals */
    Loc_enumReturnStatus = USART_Init();

    /* Return the status of the USART initialization */
    return Loc_enumReturnStatus;
}

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
Error_enumStatus_t HUART_SendBuffAsync(HUSART_UserReq_t *Ptr_HUARTSendReq)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Check if the pointer to the UART send request structure is valid */
    if (Ptr_HUARTSendReq == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        switch (Ptr_HUARTSendReq->USART_ID)
        {
        case HUSART1_ID:
            g_Index_Of_Sending = g_UART1_idx;
            break;
        case HUSART2_ID:
            g_Index_Of_Sending = g_UART2_idx;
            break;
        case HUSART6_ID:
            g_Index_Of_Sending = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        /* Check if the UART send request state is ready to send data */
        if (Loc_enumReturnStatus == Status_enumOk)
        {
            SendReq[g_Index_Of_Sending].BuffReqInfo.Ptr_buffer = Ptr_HUARTSendReq->Ptr_buffer;
            SendReq[g_Index_Of_Sending].BuffReqInfo.Buff_Len = Ptr_HUARTSendReq->Buff_Len;
            SendReq[g_Index_Of_Sending].BuffReqInfo.Buff_cb = Ptr_HUARTSendReq->Buff_cb;
            SendReq[g_Index_Of_Sending].BuffReqInfo.USART_ID = Ptr_HUARTSendReq->USART_ID;
            USART_TxBufferAsyncZeroCopy(&(SendReq[g_Index_Of_Sending].BuffReqInfo));
        }
        else
        {
            Loc_enumReturnStatus = Status_enumNotOk;
        }
    }

    /* Return the status of the UART send operation initiation */
    return Loc_enumReturnStatus;
}

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
Error_enumStatus_t HUART_ReceiveBuffAsync(HUSART_UserReq_t *Ptr_HUARTGetReq)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Check if the pointer to the UART receive request structure is valid */
    if (Ptr_HUARTGetReq == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        switch (Ptr_HUARTGetReq->USART_ID)
        {
        case HUSART1_ID:
            g_Index_Of_Receiving = g_UART1_idx;
            break;
        case HUSART2_ID:
            g_Index_Of_Receiving = g_UART2_idx;
            break;
        case HUSART6_ID:
            g_Index_Of_Receiving = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        /* Check if the UART receive request state is ready to receive data */
        if (Loc_enumReturnStatus == Status_enumOk)
        {
            GetReq[g_Index_Of_Receiving].BuffReqInfo.Ptr_buffer = Ptr_HUARTGetReq->Ptr_buffer;
            GetReq[g_Index_Of_Receiving].BuffReqInfo.Buff_Len = Ptr_HUARTGetReq->Buff_Len;
            GetReq[g_Index_Of_Receiving].BuffReqInfo.Buff_cb = Ptr_HUARTGetReq->Buff_cb;
            GetReq[g_Index_Of_Receiving].BuffReqInfo.USART_ID = Ptr_HUARTGetReq->USART_ID;
            USART_RxBufferAsyncZeroCopy(&(GetReq[g_Index_Of_Sending].BuffReqInfo));
        }
        else
        {
            Loc_enumReturnStatus = Status_enumNotOk;
        }

        /* Return the status of the UART receive operation initiation */
        
    }
    return Loc_enumReturnStatus;
}
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
#include "HUSART.h"
#include "../../MCAL/USART/USART.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/NVIC/NVIC.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define UART_PINS_NUM 2
#define TX_ID 0
#define RX_ID 1

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
// typedef void (*Send_CBF_t)(void);
// typedef void (*GetCBF_t)(void);

// typedef enum
// {
//     HUSART_ReqReady,
//     HUSART_ReqBusy
// } HUSART_UserRequestState;

// typedef enum
// {
//     HUSART_NoReq,
//     HUSART_Send,
//     HUSART_Get,
//     HUSART_HandleReq 
// } HUSART_UserRequestType;

typedef struct
{
    USART_UserReq_t BuffReqInfo;
    //HUSART_UserRequestState state;

} HUSART_SendReq_t;
typedef struct
{
    USART_UserReq_t BuffReqInfo;
    //HUSART_UserRequestState state;
} HUSART_GetReq_t;
/*******************************************************************************
 *                              Variables                                       *
 *******************************************************************************/
HUSART_SendReq_t SendReq[_USART_Num];
HUSART_GetReq_t GetReq[_USART_Num];
//static HUSART_UserRequestType g_HUART_Type= HUSART_NoReq;
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
 * @brief    : Handles the UART tasks based on its current type.
 * @details  : This function checks the current type of UART operation and performs the corresponding actions.
 *             - If the UART type is HUSART_Send and there's a pending send request:
 *               - If the send request state is HUSART_ReqBusy, it initiates the asynchronous transmission of data.
 *               - Checks if the transmission is done using USART_TxDone function and updates the status accordingly.
 *             - If the UART type is HUSART_Get and there's a pending receive request:
 *               - If the receive request state is HUSART_ReqBusy, it initiates the asynchronous reception of data.
 *               - Checks if data reception is complete using USART_IsRx function and updates the status accordingly.
 *             - If the UART type is HUSART_NoReq, no action is taken.
 * @param    : None
 * @return   : None
 **/
// void HUART_Runnable(void)
// {
//     switch (g_HUART_Type)
//     {
//     case HUSART_HandleReq:
//         /* Checks if there's a pending send request */
//         if (SendReq[g_Index_Of_Sending].state == HUSART_ReqBusy)
//         {
//             /* Initiates asynchronous transmission of data */
//             USART_TxBufferAsyncZeroCopy(&(SendReq[g_Index_Of_Sending].BuffReqInfo));
//             SendReq[g_Index_Of_Sending].state = HUSART_ReqReady;
//             g_HUART_Type = HUSART_NoReq ;
//        }
//             /* Checks if there's a pending receive request */
//         if (GetReq[g_Index_Of_Receiving].state == HUSART_ReqBusy)
//         {
//             /* Initiates asynchronous reception of data */
//             USART_RxBufferAsyncZeroCopy(&(GetReq[g_Index_Of_Receiving].BuffReqInfo));
//             GetReq[g_Index_Of_Receiving].state = HUSART_ReqReady; 
//             g_HUART_Type = HUSART_NoReq ;
//         }
//         break;       
//     case HUSART_NoReq:
//         /* No action is taken when the HUART is turned off */
//         /* Do Nothing */
//         break;
        
//     default:
//         break;
//     }
// }

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
    GPIO_pinConfig_t UART_PINS[UART_PINS_NUM] = {
        [TX_ID] = {.mode = GPIO_MODE_ALTERNATE_OPENDRAIN_NOPULL, .speed = GPIO_HIGH_SPEED},
        [RX_ID] = {.mode = GPIO_MODE_ALTERNATE_OPENDRAIN_NOPULL, .speed = GPIO_HIGH_SPEED}};
    
    /* Initialize GPIO pins for UART TX and RX */
    for (Loc_idx = 0; Loc_idx < _USART_Num; Loc_idx++)
    {
        UART_PINS[TX_ID].port = HUARTS[Loc_idx].TX_PORT;
        UART_PINS[TX_ID].pin = HUARTS[Loc_idx].TX_PIN;
        UART_PINS[RX_ID].port = HUARTS[Loc_idx].RX_PORT;
        UART_PINS[RX_ID].pin = HUARTS[Loc_idx].RX_PIN;
        
        GPIO_initPin(&UART_PINS[TX_ID]);
        GPIO_initPin(&UART_PINS[RX_ID]);
        
        /* Configure GPIO alternate function for UART TX and RX */
        GPIO_configAlterFunc(UART_PINS[TX_ID].port, UART_PINS[TX_ID].pin, HUARTS[Loc_idx].TX_AF_ID);
        GPIO_configAlterFunc(UART_PINS[RX_ID].port, UART_PINS[RX_ID].pin, HUARTS[Loc_idx].TX_AF_ID);
        
        /* Enable NVIC interrupts for UART communication */
        switch (HUARTS[Loc_idx].USART_ID)
        {
        case HUSART1_ID:
            NVIC_EnableIRQ(USART1_IRQ);
            break;
        case HUSART2_ID:
            NVIC_EnableIRQ(USART2_IRQ);
            break;
        case HUSART6_ID:
            NVIC_EnableIRQ(USART6_IRQ);
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
            //SendReq[g_Index_Of_Sending].state = HUSART_ReqBusy;
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
            //GetReq[g_Index_Of_Receiving].state = HUSART_ReqBusy;
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


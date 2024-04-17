/*
 ============================================================================
 Name        : USART.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the USART Driver
 Date        : 30/3/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes                                    *
 *******************************************************************************/
#include "MCAL/UART/USART.h"
/*******************************************************************************
 *                             Definitions                                      *
 *******************************************************************************/
#define USART1_BA (void *)0x40011000
#define USART2_BA (void *)0x40004400
#define USART6_BA (void *)0x40011400
#define UART_NUMS_IN_TARGET 3
#define MANTISSA_SHIFT 4
#define UART_PRE_ENABLE_MASK 0X00002000
#define UART_TX_ENABLE_MASK 0X00000008
#define UART_TXE_ENABLE_MASK 0X00000080
#define UART_RX_ENABLE_MASK 0X00000004
#define UART_RXE_ENABLE_MASK 0X00000020
#define UART_RX_CLR_ENABLE_MASK 0XFFFFFFFB
#define UART_TX_EMPTY_FLAG 0X00000080
#define UART_RX_NOT_EMPTY_FLAG 0X00000020
#define UART_TX_DONE_FLAG 0X00000040
/*******************************************************************************
 *                            Types Declaration                                 *
 *******************************************************************************/
typedef void (*TX_CBF_t)(void);
typedef void (*RX_CBF_t)(void);
typedef enum
{
    USART_ReqReady,
    USART_ReqBusy
} USART_UserRequestState;
typedef struct
{
    uint8_t *data;
    uint32_t Pos;
    uint32_t size;
} USART_buffer_t;
typedef struct
{
    USART_buffer_t buffer;
    USART_UserRequestState state;
    TX_CBF_t CB;
} USART_TxReq_t;
typedef struct
{
    USART_buffer_t buffer;
    USART_UserRequestState state;
    TX_CBF_t CB;
} USART_RXReq_t;
typedef struct
{
    uint32_t USART_SR;
    uint32_t USART_DR;
    uint32_t USART_BRR;
    uint32_t USART_CR1;
    uint32_t USART_CR2;
    uint32_t USART_CR3;
    uint32_t USART_GTPR;

} USART_PERI_t;

/*******************************************************************************
 *                              Variables                                       *
 *******************************************************************************/
extern const USART_Config_t USARTS[_USART_Num];
volatile void *const USART[UART_NUMS_IN_TARGET] = {USART1_BA, USART2_BA, USART6_BA};
static USART_TxReq_t TxReq[_USART_Num];
static USART_RXReq_t RxReq[_USART_Num];
uint8_t g_UART1_idx;
uint8_t g_UART2_idx;
uint8_t g_UART6_idx;
/*******************************************************************************
 *                             Implementation                                   *
 *******************************************************************************/
/**
 * @brief    : Initializes USART communication.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of USART initialization.
 * @details  : This function initializes USART communication by configuring the USART peripheral,
 *             setting the baud rate, configuring frame format (word length, parity, stop bits),
 *             and enabling USART communication.
 */
Error_enumStatus_t USART_Init(void)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Local variable for loop iteration */
    uint8_t Loc_idx;
    /* Baud Rate Register value */
    uint32_t Loc_BRRValue = 0;
    /* Control Register 1 value */
    uint32_t Loc_CR1Value = 0;
    /* Control Register 2 value */
    uint32_t Loc_CR2Value = 0;
    /* Oversampling mode */
    uint32_t Loc_OVER8 = 0;
    /* USART Divider value */
    uint32_t Loc_USARTDIVValue = 0;
    /* Fractional part of the divider */
    uint32_t Loc_DIV_Fraction = 0;
    /* Mantissa part of the divider */
    uint32_t Loc_DIV_Mantissa = 0;

    /* Check if USART number is valid */
    if (_USART_Num > (USART6_ID + 1))
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Iterate over each USART */
        for (Loc_idx = 0; Loc_idx < _USART_Num; Loc_idx++)
        {
            /**
             * Calculate USARTDIV value to set the required Tx/Rx baud as the equations is
             * Tx/Rx baud Rate = F_CK / ( 8 × (2 – OVER8) × USARTDIV) )
             */
            /** Calculate oversampling mode value
             * As if i use over sample by 8 the  Loc_OVER8 value will equal 1 otherwise it will equal zero**/
            Loc_OVER8 = USARTS[Loc_idx].OverSamplingMode / USART_OVS_8;
            /** Calculate USART Divider value Multiplied by 100 to get the first to fraction digits
             * As if USARTDIV is equal to 50.99 i make it equals 5099
             */
            Loc_USARTDIVValue = (FCPU * 100) / (8 * (2 - Loc_OVER8) * USARTS[Loc_idx].BaudRate);
            /** Calculate fractional part of divider from the below equation :
             * DIV_Fraction = ( 8 × (2 – OVER8) * fraction part which it is the first 2 digits from
             * USARTDIV value after multiply it by 100
             */
            Loc_DIV_Fraction = (Loc_USARTDIVValue % 100) * (8 * (2 - Loc_OVER8));
            /* Adjust fractional part i get the nearest real number for it */
            if (Loc_DIV_Fraction % 100 != 0)
            {
                /*Got the real value of DIV_Factor after divid it by 100 */
                Loc_DIV_Fraction = ((Loc_DIV_Fraction / 100) + 1);
            }
            else
            {
                Loc_DIV_Fraction = Loc_DIV_Fraction / 100;
            }
            /* Calculate mantissa part of divider as it uqual the real part of  USARTDIV value so i divid it by 100 */
            Loc_DIV_Mantissa = Loc_USARTDIVValue / 100;
            /* Adjust mantissa and fractional parts if needed in case of overflow happened */
            if (((Loc_OVER8 == 1) && (Loc_DIV_Fraction == 8)) || ((Loc_OVER8 == 0) && (Loc_DIV_Fraction == 16)))
            {
                /*Set DIV_Fraction by zero and add the carry to mantissa */
                Loc_DIV_Fraction = 0;
                Loc_DIV_Mantissa++;
            }
            /* Combine mantissa and fractional parts to get BRR value */
            Loc_DIV_Mantissa = Loc_DIV_Mantissa << MANTISSA_SHIFT;
            Loc_BRRValue = Loc_DIV_Mantissa | Loc_DIV_Fraction;
            /* Configure Control Register 1 value */
            Loc_CR1Value = UART_PRE_ENABLE_MASK | USARTS[Loc_idx].OverSamplingMode | USARTS[Loc_idx].WordLength | USARTS[Loc_idx].ParityEn | USARTS[Loc_idx].ParityType;
            /* Configure Control Register 2 value */
            Loc_CR2Value = USARTS[Loc_idx].StopBits;
            /* Set BRR value */
            ((USART_PERI_t *)USART[USARTS[Loc_idx].USART_ID])->USART_BRR = Loc_BRRValue;
            /* Set CR1 value */
            ((USART_PERI_t *)USART[USARTS[Loc_idx].USART_ID])->USART_CR1 = Loc_CR1Value;
            /* Set CR2 value */
            ((USART_PERI_t *)USART[USARTS[Loc_idx].USART_ID])->USART_CR2 = Loc_CR2Value;
            switch (USARTS[Loc_idx].USART_ID)
            {
            case USART1_ID:
                g_UART1_idx = Loc_idx;
                break;
            case USART2_ID:
                g_UART2_idx = Loc_idx;
                break;
            case USART6_ID:
                g_UART6_idx = Loc_idx;
                break;
            default:
                Loc_enumReturnStatus = Status_enumNotOk;
                break;
            }
        }
    }

    /* Return the status of the USART initialization */
    return Loc_enumReturnStatus;
}

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
Error_enumStatus_t USART_TxBufferAsyncZeroCopy(USART_UserReq_t *Ptr_UserReq)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_Reqidx =0;
    /* Check for NULL pointer */
    if (Ptr_UserReq == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {

        switch (Ptr_UserReq->USART_ID)
        {
        case USART1_ID:
            Loc_Reqidx = g_UART1_idx;
            break;
        case USART2_ID:
            Loc_Reqidx = g_UART2_idx;
            break;
        case USART6_ID:
            Loc_Reqidx = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        if (TxReq[Loc_Reqidx].state == USART_ReqReady)
        {
            /* Set transmit request state to busy */
            TxReq[Loc_Reqidx].state = USART_ReqBusy;
            /* Copy transmit buffer parameters from user request */
            TxReq[Loc_Reqidx].buffer.data = Ptr_UserReq->Ptr_buffer;
            TxReq[Loc_Reqidx].buffer.size = Ptr_UserReq->Buff_Len;
            TxReq[Loc_Reqidx].buffer.Pos = 0;
            TxReq[Loc_Reqidx].CB = Ptr_UserReq->Buff_cb;
            /* Enable USART transmit */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 |= UART_TX_ENABLE_MASK;
            /* Load first byte of data into USART data register */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_DR = TxReq[Loc_Reqidx].buffer.data[0];
            TxReq[Loc_Reqidx].buffer.Pos++;
            /* Enable USART transmit data register empty interrupt */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 |= UART_TXE_ENABLE_MASK;
        }
        else
        {
            Loc_enumReturnStatus = Status_enumBusyState;
        }
    }
    /* Return the status of the transmission */
    return Loc_enumReturnStatus;
}

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
Error_enumStatus_t USART_RxBufferAsyncZeroCopy(USART_UserReq_t *Ptr_UserReq)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_Reqidx =0;
    /* Check for NULL pointer */
    if (Ptr_UserReq == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        switch (Ptr_UserReq->USART_ID)
        {
        case USART1_ID:
            Loc_Reqidx = g_UART1_idx;
            break;
        case USART2_ID:
            Loc_Reqidx = g_UART2_idx;
            break;
        case USART6_ID:
            Loc_Reqidx = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        if (RxReq[Loc_Reqidx].state == USART_ReqReady)
        {
            /* Clear RXNE flag */
           // ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 &= UART_RX_CLR_ENABLE_MASK;
            /* Set receive request state to busy */
            RxReq[Loc_Reqidx].state = USART_ReqBusy;
            /* Copy receive buffer parameters from user request */
            RxReq[Loc_Reqidx].buffer.data = Ptr_UserReq->Ptr_buffer;
            RxReq[Loc_Reqidx].buffer.size = Ptr_UserReq->Buff_Len;
            RxReq[Loc_Reqidx].buffer.Pos = 0;
            RxReq[Loc_Reqidx].CB = Ptr_UserReq->Buff_cb;
            /* Enable USART receive */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 |= UART_RX_ENABLE_MASK;
            /* Enable USART receive data register not empty interrupt */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 |= UART_RXE_ENABLE_MASK;
        }
        else
        {
            Loc_enumReturnStatus = Status_enumBusyState;
        }
    }

    /* Return the status of the reception */
    return Loc_enumReturnStatus;
}

/**
 * @brief    : Transmits a single byte over USART.
 * @param[in]: Ptr_UserReq Pointer to USART user request structure containing transmit parameters.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the transmission.
 * @details  : This function transmits a single byte of data over USART.
 *             It checks for NULL pointer input and the length of the transmit buffer.
 *             If the USART transmit request is ready, it sets the request state to busy,
 *             transmits the byte of data, and waits for the transmission to complete.
 **/
Error_enumStatus_t USART_SendByte(USART_UserReq_t *Ptr_UserReq)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_Reqidx = 0;
    /* Check for NULL pointer */
    if (Ptr_UserReq == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else if (Ptr_UserReq->Buff_Len > 1)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        switch (Ptr_UserReq->USART_ID)
        {
        case USART1_ID:
            Loc_Reqidx = g_UART1_idx;
            break;
        case USART2_ID:
            Loc_Reqidx = g_UART2_idx;
            break;
        case USART6_ID:
            Loc_Reqidx = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        if (TxReq[Loc_Reqidx].state == USART_ReqReady)
        {
            volatile uint16_t Time = 2000;
            /* Set transmit request state to busy */
            TxReq[Loc_Reqidx].state = USART_ReqBusy;
            /* Transmit the byte of data */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_DR = *(Ptr_UserReq->Ptr_buffer);
            /* Enable USART transmit */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 |= UART_TX_ENABLE_MASK;
            /* Wait for transmission to complete */
            while ((((((USART_PERI_t *)USART[Loc_Reqidx])->USART_SR) & (UART_TX_EMPTY_FLAG)) == 0) && Time)
            {
                Time--;
            }
            if (Time == 0)
            {
                if (((((USART_PERI_t *)USART[Loc_Reqidx])->USART_SR) & UART_TX_EMPTY_FLAG) == 0)
                {
                    Loc_enumReturnStatus = Status_enumTimOut;
                }
            }
            /* Set transmit request state back to ready */
            TxReq[Loc_Reqidx].state = USART_ReqReady;
        }
        else
        {
            Loc_enumReturnStatus = Status_enumBusyState;
        }
    }
    /* Return the status of the transmission */
    return Loc_enumReturnStatus;
}

/**
 * @brief    : Receives a single byte over USART.
 * @param[in]: Ptr_UserReq Pointer to USART user request structure containing receive parameters.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the reception.
 * @details  : This function receives a single byte of data over USART.
 *             It checks for NULL pointer input and the length of the receive buffer.
 *             If the USART receive request is ready, it sets the request state to busy,
 *             enables USART receive, waits for a byte of data to be received, and reads the received byte.
 **/
Error_enumStatus_t USART_GetByte(USART_UserReq_t *Ptr_UserReq)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_Reqidx =0 ;
    /* Check for NULL pointer */
    if (Ptr_UserReq == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else if (Ptr_UserReq->Buff_Len > 1)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        switch (Ptr_UserReq->USART_ID)
        {
        case USART1_ID:
            Loc_Reqidx = g_UART1_idx;
            break;
        case USART2_ID:
            Loc_Reqidx = g_UART2_idx;
            break;
        case USART6_ID:
            Loc_Reqidx = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        if (RxReq[Loc_Reqidx].state == USART_ReqReady)
        {
            volatile uint16_t Time = 2000;
            /* Set receive request state to busy */
            RxReq[Loc_Reqidx].state = USART_ReqBusy;
            /* Enable USART receive */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 |= UART_RX_ENABLE_MASK;
            /* Wait for a byte of data to be received */
            while ((((((USART_PERI_t *)USART[Loc_Reqidx])->USART_SR) & UART_RX_NOT_EMPTY_FLAG) == 0) && Time)
            {
                Time--;
            }
            if (Time == 0)
            {
                if (((((USART_PERI_t *)USART[Loc_Reqidx])->USART_SR) & UART_RX_NOT_EMPTY_FLAG) == 0)
                {
                    Loc_enumReturnStatus = Status_enumTimOut;
                }
                else
                {
                    /* Read the received byte */
                    *(Ptr_UserReq->Ptr_buffer) = ((USART_PERI_t *)USART[Loc_Reqidx])->USART_DR;
                }
            }
            else
            {
                /* Read the received byte */
                *(Ptr_UserReq->Ptr_buffer) = ((USART_PERI_t *)USART[Loc_Reqidx])->USART_DR;
            }
            /* Disable USART receive */
            ((USART_PERI_t *)USART[Loc_Reqidx])->USART_CR1 &= ~UART_RX_ENABLE_MASK;
            /* Set receive request state back to ready */
            RxReq[Loc_Reqidx].state = USART_ReqReady;
        }
        else
        {
            Loc_enumReturnStatus = Status_enumBusyState;
        }
    }
    /* Return the status of the reception */
    return Loc_enumReturnStatus;
}

/**
 * @brief    : Checks if USART transmission is completed.
 * @param[in]: USART_ID   USART ID.
 * @param[out]: Ptr_Status Pointer to a variable to store the status of transmission completion.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the operation.
 * @details  : This function checks if the transmission over the specified USART is completed.
 *             It reads the USART status register to determine the transmission status.
 **/
Error_enumStatus_t USART_TxDone(uint8_t USART_ID, uint8_t *Ptr_Status)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_Reqidx = 0;
    /* Check for NULL pointer */
    if (Ptr_Status == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        switch (USART_ID)
        {
        case USART1_ID:
            Loc_Reqidx = g_UART1_idx;
            break;
        case USART2_ID:
            Loc_Reqidx = g_UART2_idx;
            break;
        case USART6_ID:
            Loc_Reqidx = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        /* Check if transmission is completed */
        if ((((((USART_PERI_t *)USART[Loc_Reqidx])->USART_SR) & UART_TX_DONE_FLAG) != 0))
        {
            *Ptr_Status = Done;
        }
        else
        {
            *Ptr_Status = NOT_Done;
        }
    }

    /* Return the status of the operation */
    return Loc_enumReturnStatus;
}

/**
 * @brief    : Checks if USART is ready to receive data.
 * @param[in]: USART_ID   USART ID.
 * @param[out]: Ptr_Status Pointer to a variable to store the status of USART reception.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of the operation.
 * @details  : This function checks if the specified USART is ready to receive data.
 *             It reads the USART status register to determine the reception status.
 **/
Error_enumStatus_t USART_IsRx(uint8_t USART_ID, uint8_t *Ptr_Status)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint8_t Loc_Reqidx = 0;
    /* Check for NULL pointer */
    if (Ptr_Status == NULL)
    {
        Loc_enumReturnStatus = Status_enumNULLPointer;
    }
    else
    {
        switch (USART_ID)
        {
        case USART1_ID:
            Loc_Reqidx = g_UART1_idx;
            break;
        case USART2_ID:
            Loc_Reqidx = g_UART2_idx;
            break;
        case USART6_ID:
            Loc_Reqidx = g_UART6_idx;
            break;
        default:
            Loc_enumReturnStatus = Status_enumNotOk;
            break;
        }
        /* Check if USART is ready to receive data */
        if ((((((USART_PERI_t *)USART[Loc_Reqidx])->USART_SR) & UART_RX_NOT_EMPTY_FLAG) != 0))
        {
            *Ptr_Status = Done;
        }
        else
        {
            *Ptr_Status = NOT_Done;
        }
    }

    /* Return the status of the operation */
    return Loc_enumReturnStatus;
}

/**
 * @brief    : USART1 interrupt handler.
 * @details  : This function handles the USART1 interrupt.
 *             It checks if the transmission or reception is complete and performs the necessary actions.
 **/
void USART1_IRQHandler(void)
{
    /* Local Variable to store CR1 value */
    uint32_t Lo_CR1_Value = ((USART_PERI_t *)USART[g_UART1_idx])->USART_CR1;

    /* Check if USART transmission is empty */
    if ((((USART_PERI_t *)USART[g_UART1_idx])->USART_SR) & UART_TX_EMPTY_FLAG)
    {
        /* Check if there are more bytes to transmit */
        if ((TxReq[g_UART1_idx].buffer.Pos) < (TxReq[g_UART1_idx].buffer.size))
        {
            /* Transmit the next byte */
            ((USART_PERI_t *)USART[g_UART1_idx])->USART_DR = TxReq[g_UART1_idx].buffer.data[TxReq[g_UART1_idx].buffer.Pos];
            TxReq[g_UART1_idx].buffer.Pos++;
        }
        else
        {
            /* Disable TXE interrupt */
            Lo_CR1_Value &= ~(UART_TXE_ENABLE_MASK);
            TxReq[g_UART1_idx].state = USART_ReqReady;
            ((USART_PERI_t *)USART[g_UART1_idx])->USART_CR1 = Lo_CR1_Value;
            /* Call callback function if available */
            if (TxReq[g_UART1_idx].CB)
            {
                TxReq[g_UART1_idx].CB();
            }
        }
    }
    /* Check if USART reception is not empty */
    if ((((USART_PERI_t *)USART[g_UART1_idx])->USART_SR) & UART_RX_NOT_EMPTY_FLAG)
    {
        /* Check if there are more bytes to receive */
        if (RxReq[g_UART1_idx].buffer.Pos < RxReq[g_UART1_idx].buffer.size)
        {
            /* Receive the next byte */
            RxReq[g_UART1_idx].buffer.data[RxReq[g_UART1_idx].buffer.Pos] = ((USART_PERI_t *)USART[g_UART1_idx])->USART_DR;
            RxReq[g_UART1_idx].buffer.Pos++;
            /* Check if all bytes are received */
            if (RxReq[g_UART1_idx].buffer.Pos == RxReq[g_UART1_idx].buffer.size)
            {
                RxReq[g_UART1_idx].state = USART_ReqReady;
                RxReq[g_UART1_idx].buffer.Pos =0 ;
                /* Disable RXE interrupt */
                //((USART_PERI_t *)USART[g_UART1_idx])->USART_CR1 &= ~UART_RXE_ENABLE_MASK;
                /* Call callback function if available */
                if (RxReq[g_UART1_idx].CB)
                {
                    RxReq[g_UART1_idx].CB();
                }
            }
        }
    }
}

/**
 * @brief    : USART2 interrupt handler.
 * @details  : This function handles the USART2 interrupt.
 *             It checks if the transmission or reception is complete and performs the necessary actions.
 **/
void USART2_IRQHandler(void)
{
    /* Local Variable to store CR1 value */
    uint32_t Lo_CR1_Value = ((USART_PERI_t *)USART[g_UART2_idx])->USART_CR1;

    /* Check if USART transmission is empty */
    if ((((USART_PERI_t *)USART[g_UART2_idx])->USART_SR) & UART_TX_EMPTY_FLAG)
    {
        /* Check if there are more bytes to transmit */
        if ((TxReq[g_UART2_idx].buffer.Pos) < (TxReq[g_UART2_idx].buffer.size))
        {
            /* Transmit the next byte */
            ((USART_PERI_t *)USART[g_UART2_idx])->USART_DR = TxReq[g_UART2_idx].buffer.data[TxReq[g_UART2_idx].buffer.Pos];
            TxReq[g_UART2_idx].buffer.Pos++;
        }
        else
        {
            /* Disable TXE interrupt */
            Lo_CR1_Value &= ~(UART_TXE_ENABLE_MASK);
            TxReq[g_UART2_idx].state = USART_ReqReady;
            ((USART_PERI_t *)USART[g_UART2_idx])->USART_CR1 = Lo_CR1_Value;
            /* Call callback function if available */
            if (TxReq[g_UART2_idx].CB)
            {
                TxReq[g_UART2_idx].CB();
            }
        }
    }
    /* Check if USART reception is not empty */
    if ((((USART_PERI_t *)USART[g_UART2_idx])->USART_SR) & UART_RX_NOT_EMPTY_FLAG)
    {
        /* Check if there are more bytes to receive */
        if (RxReq[g_UART2_idx].buffer.Pos < RxReq[g_UART2_idx].buffer.size)
        {
            /* Receive the next byte */
            RxReq[g_UART2_idx].buffer.data[RxReq[g_UART2_idx].buffer.Pos] = ((USART_PERI_t *)USART[g_UART2_idx])->USART_DR;
            RxReq[g_UART2_idx].buffer.Pos++;
            /* Check if all bytes are received */
            if (RxReq[g_UART2_idx].buffer.Pos == RxReq[g_UART2_idx].buffer.size)
            {
                RxReq[g_UART2_idx].state = USART_ReqReady;
                /* Disable RXE interrupt */
                //((USART_PERI_t *)USART[g_UART2_idx])->USART_CR1 &= ~UART_RXE_ENABLE_MASK;
                /* Call callback function if available */
                if (RxReq[g_UART2_idx].CB)
                {
                    RxReq[g_UART2_idx].CB();
                }
            }
        }
    }
}

/**
 * @brief    : USART6 interrupt handler.
 * @details  : This function handles the USART6 interrupt.
 *             It checks if the transmission or reception is complete and performs the necessary actions.
 **/
void USART6_IRQHandler(void)
{
    /* Local Variable to store CR1 value */
    uint32_t Lo_CR1_Value = ((USART_PERI_t *)USART[g_UART6_idx])->USART_CR1;

    /* Check if USART transmission is empty */
    if ((((USART_PERI_t *)USART[g_UART6_idx])->USART_SR) & UART_TX_EMPTY_FLAG)
    {
        /* Check if there are more bytes to transmit */
        if ((TxReq[g_UART6_idx].buffer.Pos) < (TxReq[g_UART6_idx].buffer.size))
        {
            /* Transmit the next byte */
            ((USART_PERI_t *)USART[g_UART6_idx])->USART_DR = TxReq[g_UART6_idx].buffer.data[TxReq[g_UART6_idx].buffer.Pos];
            TxReq[g_UART6_idx].buffer.Pos++;
        }
        else
        {
            /* Disable TXE interrupt */
            Lo_CR1_Value &= ~(UART_TXE_ENABLE_MASK);
            TxReq[g_UART6_idx].state = USART_ReqReady;
            ((USART_PERI_t *)USART[g_UART6_idx])->USART_CR1 = Lo_CR1_Value;
            /* Call callback function if available */
            if (TxReq[g_UART6_idx].CB)
            {
                TxReq[g_UART6_idx].CB();
            }
        }
    }
    /* Check if USART reception is not empty */
    if ((((USART_PERI_t *)USART[g_UART6_idx])->USART_SR) & UART_RX_NOT_EMPTY_FLAG)
    {
        /* Check if there are more bytes to receive */
        if (RxReq[g_UART6_idx].buffer.Pos < RxReq[g_UART6_idx].buffer.size)
        {
            /* Receive the next byte */
            RxReq[g_UART6_idx].buffer.data[RxReq[g_UART6_idx].buffer.Pos] = ((USART_PERI_t *)USART[g_UART6_idx])->USART_DR;
            RxReq[g_UART6_idx].buffer.Pos++;
            /* Check if all bytes are received */
            if (RxReq[g_UART6_idx].buffer.Pos == RxReq[g_UART6_idx].buffer.size)
            {
                RxReq[g_UART6_idx].state = USART_ReqReady;
                /* Disable RXE interrupt */
                //((USART_PERI_t *)USART[g_UART6_idx])->USART_CR1 &= ~UART_RXE_ENABLE_MASK;
                /* Call callback function if available */
                if (RxReq[g_UART6_idx].CB)
                {
                    RxReq[g_UART6_idx].CB();
                }
            }
        }
    }
}

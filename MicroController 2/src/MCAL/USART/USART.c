/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "USART.h"
#include "USART_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/


#define USART1_BASE_ADDRESS     ((void*)0x40011000)
#define USART2_BASE_ADDRESS     ((void*)0x40004400)
#define USART6_BASE_ADDRESS     ((void*)0x40011400)

#define USART_SR_PE_Msk    ((u32)0x00000001) // Parity error flag
#define USART_SR_FE_Msk    ((u32)0x00000002) // Framing error flag
#define USART_SR_NE_Msk    ((u32)0x00000004) // Noise detection flag
#define USART_SR_ORE_Msk   ((u32)0x00000008) // Overrun error flag
#define USART_SR_IDLE_Msk  ((u32)0x00000010) // IDLE line detected flag
#define USART_SR_RXNE_Msk  ((u32)0x00000020) // Read data register not empty flag
#define USART_SR_TC_Msk    ((u32)0x00000040) // Transmission complete flag
#define USART_SR_TXE_Msk   ((u32)0x00000080) // Transmit data register empty flag
#define USART_SR_LBD_Msk   ((u32)0x00000100) // LIN break detection flag
#define USART_SR_CTS_Msk   ((u32)0x00000200) // CTS flag


// Validation macros for USART avaliable periherals options
#define IS_NOT_USART_OPTION(option) (((option) != USART1) && ((option) != USART2) && ((option) != USART6))




// Validation macros for USART configuration options

// Word length validation macro
#define IS_NOT_USART_WORD_LENGTH(LENGTH) (!((LENGTH) == USART_WORDLENGTH_8B) && \
                                           !((LENGTH) == USART_WORDLENGTH_9B))

// Stop bits validation macro
#define IS_NOT_USART_STOPBITS(STOPBITS) (!((STOPBITS) == USART_STOPBITS_1) && \
                                           !((STOPBITS) == USART_STOPBITS_0_5) && \
                                           !((STOPBITS) == USART_STOPBITS_2) && \
                                           !((STOPBITS) == USART_STOPBITS_1_5))

// Parity validation macro
#define IS_NOT_USART_PARITY(PARITY) (!((PARITY) == USART_PARITY_NONE) && \
                                       !((PARITY) == USART_PARITY_EVEN) && \
                                       !((PARITY) == USART_PARITY_ODD))

// Mode validation macro
#define IS_NOT_USART_MODE(MODE) (!((MODE) == USART_MODE_RX) && \
                                   !((MODE) == USART_MODE_TX) && \
                                   !((MODE) == USART_MODE_TX_RX))



// Oversampling mode validation macro
#define IS_NOT_USART_OVERSAMPLING(SAMPLING) (!((SAMPLING) == USART_OVERSAMPLING_16) && \
                                               !((SAMPLING) == USART_OVERSAMPLING_8))


// Validation macros for USART interrupt masks
// RXNE interrupt enable mask validation macro
#define IS_NOT_USART_RXNEIE_OPTION(MASK) (((MASK) != USART_RXNEIE_ENABLE) && ((MASK) != USART_RXNEIE_DISABLE))

// TXE interrupt enable mask validation macro
#define IS_NOT_USART_TXEIE_OPTION(MASK) (((MASK) != USART_TXEIE_ENABLE) && ((MASK) != USART_TXEIE_DISABLE))

// TC interrupt enable mask validation macro
#define IS_NOT_USART_TCIE_OPTION(MASK) (((MASK) != USART_TCIE_ENABLE) && ((MASK) != USART_TCIE_DISABLE))

// PE interrupt enable mask validation macro
#define IS_NOT_USART_PEIE_OPTION(MASK) (((MASK) != USART_PEIE_ENABLE) && ((MASK) != USART_PEIE_DISABLE))


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct{
    volatile u32 SR;    // USART Status Register
    volatile u32 DR;    // USART Data Register
    volatile u32 BRR;   // USART Baud Rate Register
    volatile u32 CR1;   // USART Control Register 1
    volatile u32 CR2;   // USART Control Register 2
    volatile u32 CR3;   // USART Control Register 3
    volatile u32 GTPR;  // USART Guard time and prescaler Register (not available on all STM32 series)
} USART_registers_t;



typedef enum{
    USART_Ready,
    USART_Busy
}USART_TxRxState_t;

typedef struct{
    u8 *data;
    u16 size;
    u16 currPos;
}USART_bufferConfig_t;

typedef struct{
    USART_bufferConfig_t *buffer;
    USART_TxRxState_t state;
    USART_callBackFuncPtr_t cbf;
}USART_TxReq_t;

typedef struct{
    USART_bufferConfig_t *buffer;
    USART_TxRxState_t state;
    USART_callBackFuncPtr_t cbf;
}USART_RxReq_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile USART_registers_t *const USARTs[3] = {(volatile USART_registers_t *)USART1_BASE_ADDRESS,
                                             (volatile USART_registers_t *)USART2_BASE_ADDRESS,
                                             (volatile USART_registers_t *)USART6_BASE_ADDRESS};

u32 USART_ClkFreq[3] = {UART1_CLK_FREQ, UART2_CLK_FREQ, UART6_CLK_FREQ};

USART_TxReq_t TXs[3] = {{.state = USART_Ready}, {.state = USART_Ready}, {.state = USART_Ready}};
USART_RxReq_t RXs[3] = {{.state = USART_Ready}, {.state = USART_Ready}, {.state = USART_Ready}};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/*static u32 roundDoubleToInt(double dNum)
{
    u32 iNum = dNum;
    double diff = dNum - (double)iNum;
    if(diff >= (double)0.5)
    {
        iNum++;
    }
    return iNum;
}
static u32 calcBaudRate(void *instanceName, u32 baud, u32 overSampling)
{
    u32 usartBRR = 0;
    u32 DIVMant = 0, DIVfracInteger = 0;
    double usartDIV = 0.0, DIVfrac = 0.0;
    
    //overSampling >>= 15;

    if(instanceName == USART1)
    {
        if(overSampling == USART_OVERSAMPLING_8)
        {
            usartDIV = (UART1_CLK_FREQ / (8 * baud));
            DIVMant = (u32)usartDIV;
            DIVfrac = (usartDIV - ((u32)usartDIV)) * 8;
            DIVfracInteger = roundDoubleToInt(DIVfrac);
            if(DIVfracInteger > 7)
            {
                DIVMant++;
            }
            usartBRR = (DIVMant<<4) | DIVfracInteger;
        }
        else if(overSampling == USART_OVERSAMPLING_16)
        {
            usartDIV = (UART1_CLK_FREQ / (8 * 2 * baud));
            DIVMant = (u32)usartDIV;
            DIVfrac = (usartDIV - ((u32)usartDIV)) * 16;
            DIVfracInteger = roundDoubleToInt(DIVfrac);
            if(DIVfracInteger > 15)
            {
                DIVMant++;
                DIVfracInteger = 0;
            }
            usartBRR = (DIVMant<<4) | DIVfracInteger;
        }
       

    }
    else if(instanceName == USART2)
    {
        if(overSampling == USART_OVERSAMPLING_8)
        {
            usartDIV = (UART2_CLK_FREQ / (8 * baud));
            DIVMant = (u32)usartDIV;
            DIVfrac = (usartDIV - ((u32)usartDIV)) * 8;
            DIVfracInteger = roundDoubleToInt(DIVfrac);
            if(DIVfracInteger > 7)
            {
                DIVMant++;
            }
            usartBRR = (DIVMant<<4) | DIVfracInteger;
        }
        else if(overSampling == USART_OVERSAMPLING_16)
        {
            usartDIV = (UART2_CLK_FREQ / (8 * 2 * baud));
            DIVMant = (u32)usartDIV;
            DIVfrac = (usartDIV - ((u32)usartDIV)) * 16;
            DIVfracInteger = roundDoubleToInt(DIVfrac);
            if(DIVfracInteger > 15)
            {
                DIVMant++;
            }
            usartBRR = (DIVMant<<4) | DIVfracInteger;
        }
    }
    else if(instanceName == USART6)
    {
        if(overSampling == USART_OVERSAMPLING_8)
        {
            usartDIV = (UART6_CLK_FREQ / (8 * baud));
            DIVMant = (u32)usartDIV;
            DIVfrac = (usartDIV - ((u32)usartDIV)) * 8;
            DIVfracInteger = roundDoubleToInt(DIVfrac);
            if(DIVfracInteger > 7)
            {
                DIVMant++;
            }
            usartBRR = (DIVMant<<4) | DIVfracInteger;
        }
        else if(overSampling == USART_OVERSAMPLING_16)
        {
            usartDIV = (UART6_CLK_FREQ / (8 * 2 * baud));
            DIVMant = (u32)usartDIV;
            DIVfrac = (usartDIV - ((u32)usartDIV)) * 16;
            DIVfracInteger = roundDoubleToInt(DIVfrac);
            if(DIVfracInteger > 15)
            {
                DIVMant++;
            }
            usartBRR = (DIVMant<<4) | DIVfracInteger;
        }
    }

    return usartBRR;
}*/

static u32 calcBaudRate(u8 instanceIDNum, u32 baud, u32 overSampling)
{
    u32 usartBRR = 0;
    u32 DIVMant = 0, DIVfracInteger = 0;
    double usartDIV = 0.0;
    double DIVfrac = 0.0;

    // Determine UART clock frequency based on the USART instance
    u32 UART_CLK_FREQ = USART_ClkFreq[instanceIDNum];
    

    // Calculate the USART BRR value based on the oversampling mode
    if (overSampling == USART_OVERSAMPLING_8) {
        usartDIV = (double)(UART_CLK_FREQ) / (8 * baud);
        DIVfrac = (usartDIV - (u32)usartDIV) * 8;
    } else if (overSampling == USART_OVERSAMPLING_16) {
        usartDIV = (double)(UART_CLK_FREQ) / (8 * 2 * baud);
        DIVfrac = (usartDIV - (u32)usartDIV) * 16;
    }

    DIVMant = (u32)usartDIV;
    DIVfracInteger = (u32)(DIVfrac + 0.5); // Round fractional part to nearest integer

    if (DIVfracInteger > (overSampling == USART_OVERSAMPLING_8 ? 7 : 15)) {
        DIVMant++;
        DIVfracInteger = 0; // Carry over, fractional part becomes 0
    }

    // Construct the USART BRR value
    usartBRR = (DIVMant << 4) | DIVfracInteger;

    return usartBRR;
}



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


USART_errorState_t USART_init(USART_instanceConfig_t *instance)
{
    USART_errorState_t errorState = USART_Nok;
    if(instance == NULLPTR)
    {
        errorState = USART_NullPtr;
    }
    else if(IS_NOT_USART_OPTION(instance->IDNum) ||
            IS_NOT_USART_MODE(instance->Mode) || 
            IS_NOT_USART_WORD_LENGTH(instance->WordLength) ||
            IS_NOT_USART_STOPBITS(instance->StopBits) ||
            IS_NOT_USART_PARITY(instance->Parity) ||
            IS_NOT_USART_OVERSAMPLING(instance->OverSampling) ||
            IS_NOT_USART_RXNEIE_OPTION(instance->RxneInterrupt) ||
            IS_NOT_USART_TXEIE_OPTION(instance->TxeInterrupt) ||
            IS_NOT_USART_TCIE_OPTION(instance->TcInterrupt))
    {
        errorState = USART_InvalidParamter;
    }
    else
    {
        errorState = USART_Ok;

        volatile USART_registers_t *const USART = (volatile USART_registers_t *)USARTs[(instance->IDNum)];
        u32 tempCR1 = USART->CR1;
        u32 tempCR2 = USART->CR2;

        tempCR1 |=   instance->Mode |
                     instance->WordLength |
                     instance->OverSampling |
                     instance->Parity |
                     instance->RxneInterrupt |
                     instance->TxeInterrupt |
                     instance->TcInterrupt;

        tempCR2 &= ~(USRT_MASK_STOPBITS);
        tempCR2 |= instance->StopBits;

        USART->BRR = calcBaudRate(instance->IDNum, instance->BaudRate, instance->OverSampling);
        USART->CR1 = tempCR1;
        USART->CR2 = tempCR2;
    }

    return errorState;
}

USART_errorState_t USART_sendByte(u8 UART_NUM, u8 data)
{
    USART_errorState_t errorState = USART_Nok;
    volatile USART_registers_t *const USART = (volatile USART_registers_t*) USARTs[UART_NUM];
    u32 timeOut = 3000;

    if(IS_NOT_USART_OPTION(UART_NUM))
    {
        errorState = USART_NullPtr;
    }
    else
    {
        errorState = USART_Ok;
        if(TXs[UART_NUM].state == USART_Ready)
        {
            TXs[UART_NUM].state = USART_Busy;
            USART->DR = data;
            while(timeOut && (!(USART->SR & USART_SR_TXE_Msk)))
                timeOut--;
            TXs[UART_NUM].state = USART_Ready;
        }
        if(timeOut == 0)
        {
           errorState = USART_TxFailed; 
        }
    }
    return errorState;
}

USART_errorState_t USART_recieveByte(u8 UART_NUM, u8 *data)
{
    USART_errorState_t errorState = USART_Nok;
    volatile USART_registers_t *const USART = USARTs[UART_NUM];
    u32 timeOut = 3000;

    if(IS_NOT_USART_OPTION(UART_NUM))
    {
        errorState = USART_NullPtr;
    }
    else
    {
        errorState = USART_Ok;
        if(RXs[UART_NUM].state == USART_Ready)
        {
            RXs[UART_NUM].state = USART_Busy;
            while(timeOut && (!(USART->SR & USART_SR_RXNE_Msk)))
                timeOut--;
            if(timeOut)
            {
                *data = (u8)USART->DR;
            }
            else
            {
                errorState = USART_RxFailed; 
            }
            RXs[UART_NUM].state = USART_Ready;
        }
        else
        {
            errorState = USART_RxFailed;
        }
    }

    return errorState;
}

USART_errorState_t USART_sendBufferAsync(u8 UART_NUM, u8 *buffer, u16 bufLen, USART_callBackFuncPtr_t cb)
{
    USART_errorState_t errorState = USART_Nok;
    volatile USART_registers_t *const USART = USARTs[UART_NUM];
    if(buffer == NULLPTR || cb == NULLPTR)
    {
        errorState = USART_NullPtr;
    }
    else if(IS_NOT_USART_OPTION(UART_NUM))
    {
        errorState = USART_InvalidParamter;
    }
    else if(TXs[UART_NUM].state == USART_Busy)
    {
        errorState = USART_TxFailed;
    }
    else
    {
        TXs[UART_NUM].state = USART_Busy;
        TXs[UART_NUM].cbf = cb;
        TXs[UART_NUM].buffer->data = buffer;
        TXs[UART_NUM].buffer->size = bufLen;
        TXs[UART_NUM].buffer->currPos = 0;
        USART->CR1 |= USART_TXEIE_ENABLE;
        USART->DR = TXs[UART_NUM].buffer->data[0];
        TXs[UART_NUM].buffer->currPos = 1;
        errorState = USART_Ok;
    }

    return errorState;
}

USART_errorState_t USART_recieveBufferAsync(u8 UART_NUM, u8 *buffer, u16 bufLen, USART_callBackFuncPtr_t cb)
{
    USART_errorState_t errorState = USART_Nok;
    volatile USART_registers_t *const USART = USARTs[UART_NUM];
    if(buffer == NULLPTR || cb == NULLPTR)
    {
        errorState = USART_NullPtr;
    }
    else if(IS_NOT_USART_OPTION(UART_NUM))
    {
        errorState = USART_InvalidParamter;
    }
    else if(RXs[UART_NUM].state == USART_Busy)
    {
        errorState = USART_RxFailed;
    }
    else
    {
        RXs[UART_NUM].state = USART_Busy;
        RXs[UART_NUM].cbf = cb;
        RXs[UART_NUM].buffer->data = buffer;
        RXs[UART_NUM].buffer->size = bufLen;
        RXs[UART_NUM].buffer->currPos = 0;
        RXs[UART_NUM].buffer->data[0] = USART->DR;
        RXs[UART_NUM].buffer->currPos = 1;;
        USART->SR &= ~(USART_SR_RXNE_Msk);
        USART->CR1 |= USART_RXNEIE_ENABLE;
        errorState = USART_Ok;
    }
    return errorState;
}

void USART1_IRQHandler(void)
{
    volatile USART_registers_t *const USART = (volatile USART_registers_t*) USARTs[USART1];
    if(USARTs[USART1]->SR & USART_SR_TXE_Msk)
    {
        if(TXs[USART1].buffer->currPos < TXs[USART1].buffer->size)
        {
            USART->DR = TXs[USART1].buffer->data[TXs[USART1].buffer->currPos];
            TXs[USART1].buffer->currPos++;
        }
        else
        {
            USART->CR1 &= ~(USART_TXEIE_ENABLE);
            TXs[USART1].state = USART_Ready;
            if(TXs[USART1].cbf)
            {
                TXs[USART1].cbf();
            }
        }   
    }

    if(USARTs[USART1]->SR & USART_SR_RXNE_Msk)
    {
        if(RXs[USART1].buffer->currPos < RXs[USART1].buffer->size)
        {
            RXs[USART1].buffer->data[RXs[USART1].buffer->currPos] = USART->DR;
            RXs[USART1].buffer->currPos++;
            if(RXs[USART1].buffer->currPos == RXs[USART1].buffer->size)
            {
                USART->CR1 &= ~(USART_RXNEIE_ENABLE);
                //RXs[1].buffer->data = NULLPTR;
                RXs[USART1].state = USART_Ready;
                if(RXs[USART1].cbf)
                {
                    RXs[USART1].cbf();
                }
            }
        }
    }
}

void USART2_IRQHandler(void)
{
    volatile USART_registers_t *const USART = (volatile USART_registers_t*) USARTs[USART2];
    if(USARTs[USART2]->SR & USART_SR_TXE_Msk)
    {
        if(TXs[USART2].buffer->currPos < TXs[USART2].buffer->size)
        {
            USART->DR = TXs[USART2].buffer->data[TXs[USART2].buffer->currPos];
            TXs[USART2].buffer->currPos++;
        }
        else
        {
            USART->CR1 &= ~(USART_TXEIE_ENABLE);
            TXs[USART2].state = USART_Ready;
            if(TXs[USART2].cbf)
            {
                TXs[USART2].cbf();
            }
        }   
    }

    if(USARTs[USART2]->SR & USART_SR_RXNE_Msk)
    {
        if(RXs[USART2].buffer->currPos < RXs[USART2].buffer->size)
        {
            RXs[USART2].buffer->data[RXs[USART2].buffer->currPos] = USART->DR;
            RXs[USART2].buffer->currPos++;
            if(RXs[USART2].buffer->currPos == RXs[USART2].buffer->size)
            {
                USART->CR1 &= ~(USART_RXNEIE_ENABLE);
                //RXs[1].buffer->data = NULLPTR;
                RXs[USART2].state = USART_Ready;
                if(RXs[USART2].cbf)
                {
                    RXs[USART2].cbf();
                }
            }
        }
    }
}

void USART6_IRQHandler(void)
{
    volatile USART_registers_t *const USART = (volatile USART_registers_t*) USARTs[USART6];
    if(USARTs[USART6]->SR & USART_SR_TXE_Msk)
    {
        if(TXs[USART6].buffer->currPos < TXs[USART6].buffer->size)
        {
            USART->DR = TXs[USART6].buffer->data[TXs[USART6].buffer->currPos];
            TXs[USART6].buffer->currPos++;
        }
        else
        {
            USART->CR1 &= ~(USART_TXEIE_ENABLE);
            TXs[USART6].state = USART_Ready;
            if(TXs[USART6].cbf)
            {
                TXs[USART6].cbf();
            }
        }   
    }

    if(USARTs[USART6]->SR & USART_SR_RXNE_Msk)
    {
        if(RXs[USART6].buffer->currPos < RXs[USART6].buffer->size)
        {
            RXs[USART6].buffer->data[RXs[USART6].buffer->currPos] = USART->DR;
            RXs[USART6].buffer->currPos++;
            if(RXs[USART6].buffer->currPos == RXs[USART6].buffer->size)
            {
                USART->CR1 &= ~(USART_RXNEIE_ENABLE);
                //RXs[1].buffer->data = NULLPTR;
                RXs[USART6].state = USART_Ready;
                if(RXs[USART6].cbf)
                {
                    RXs[USART6].cbf();
                }
            }
        }
    }
}
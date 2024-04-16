/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "StdTypes.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

// Masks for all bits of USART CR1 register
#define USART_CR1_SBK       ((u32)0x00000001) // Send Break
#define USART_CR1_RWU       ((u32)0x00000002) // Receiver wakeup
#define USART_CR1_RE        ((u32)0x00000004) // Receiver enable
#define USART_CR1_TE        ((u32)0x00000008) // Transmitter enable
#define USART_CR1_IDLEIE    ((u32)0x00000010) // IDLE interrupt enable
#define USART_CR1_RXNEIE    ((u32)0x00000020) // RXNE interrupt enable
#define USART_CR1_TCIE      ((u32)0x00000040) // Transmission complete interrupt enable
#define USART_CR1_TXEIE     ((u32)0x00000080) // TXE interrupt enable
#define USART_CR1_PEIE      ((u32)0x00000100) // PE interrupt enable
#define USART_CR1_PS        ((u32)0x00000200) // Parity selection
#define USART_CR1_PCE       ((u32)0x00000400) // Parity control enable
#define USART_CR1_WAKE      ((u32)0x00000800) // Wakeup method
#define USART_CR1_M         ((u32)0x00001000) // Word length
#define USART_CR1_UE        ((u32)0x00002000) // USART enable
#define USART_CR1_OVER8     ((u32)0x00008000) // Oversampling mode
#define USART_CR2_STOP_0    ((u32)0x00001000) // Bit 12: STOP bit 0
#define USART_CR2_STOP_1    ((u32)0x00002000) // Bit 13: STOP bit 1
#define USART_CR2_STOP_1_5  ((u32)(USART_CR2_STOP_0 | USART_CR2_STOP_1)) // Bits 12 and 13: STOP bits 1.5



// Word length options
#define USART_WORDLENGTH_8B           ((u32)0x00000000) // 8 bits
#define USART_WORDLENGTH_9B           ((u32)USART_CR1_M) // 9 bits

// Stop bits options
#define USRT_MASK_STOPBITS            ((u32)0x00003000)
#define USART_STOPBITS_1              ((u32)0x00000000) // 1 stop bit
#define USART_STOPBITS_0_5            ((u32)USART_CR2_STOP_0) // 0.5 stop bit
#define USART_STOPBITS_2              ((u32)USART_CR2_STOP_1) // 2 stop bits
#define USART_STOPBITS_1_5            ((u32)(USART_CR2_STOP_0 | USART_CR2_STOP_1)) // 1.5 stop bits

// Parity options
#define USART_PARITY_NONE             ((u32)0x00000000) // No parity
#define USART_PARITY_EVEN             ((u32)USART_CR1_PCE) // Even parity
#define USART_PARITY_ODD              ((u32)(USART_CR1_PCE | USART_CR1_PS)) // Odd parity

// Mode options
#define USART_MODE_RX                 ((u32)USART_CR1_RE) // Receive mode
#define USART_MODE_TX                 ((u32)USART_CR1_TE) // Transmit mode
#define USART_MODE_TX_RX              ((u32)(USART_CR1_TE | USART_CR1_RE)) // Transmit and receive mode


// Oversampling mode options
#define USART_OVERSAMPLING_16         ((u32)0x00000000) // Oversampling by 16
#define USART_OVERSAMPLING_8          ((u32)USART_CR1_OVER8) // Oversampling by 8


// One-bit sampling method options
#define USART_ONEBIT_DISABLE          ((uint32_t)0x00000000) // One-bit sampling method disabled
#define USART_ONEBIT_ENABLE           ((uint32_t)USART_CR3_ONEBIT) // One-bit sampling method enabled


// Masks for USART_CR1 register to Control Interrupts options (RXNE, TXE, TC, PE)
#define USART_RXNEIE_ENABLE    ((u32)USART_CR1_RXNEIE) // Mask for RXNE interrupt enable bit
#define USART_RXNEIE_DISABLE   ((u32)0x00000000) // Disable RXNE interrupt

#define USART_TXEIE_ENABLE     ((u32)USART_CR1_TXEIE) // Mask for TXE interrupt enable bit
#define USART_TXEIE_DISABLE    ((u32)0x00000000)  // Disable TXE interrupt

#define USART_TCIE_ENABLE      ((u32)USART_CR1_TCIE) // Mask for Transmission complete interrupt enable bit
#define USART_TCIE_DISABLE     ((u32)0x00000000)   // Disable Transmission complete interrupt

#define USART_PEIE_ENABLE      ((u32)USART_CR1_PEIE) // Mask for Parity error interrupt enable bit
#define USART_PEIE_DISABLE     ((u32)0x00000000)   // Disable Parity error interrupt

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef enum{
    USART1,
    USART2,
    USART6
}USART_IDNum_t;

typedef struct {
    u8   IDNum;
    u32  BaudRate;            // Baud rate for communication
    u32  WordLength;          // Number of data bits transmitted or received
    u32  StopBits;            // Number of stop bits
    u32  Parity;              // Parity control mode
    u32  Mode;                // Transmission mode (e.g., TX, RX, TX/RX)
    u32  OverSampling;        // Oversampling mode (e.g., 16 or 8)
    u32  RxneInterrupt;       // RXNE interrupt enable/disable
    u32  TxeInterrupt;        // TXE interrupt enable/disable
    u32  TcInterrupt;         // Transmission complete interrupt enable/disable
}USART_instanceConfig_t;


typedef enum{
    USART_Ok,
    USART_Nok,
    USART_NullPtr,
    USART_InvalidParamter,
    USART_TxFailed,
    USART_RxFailed
}USART_errorState_t;

typedef void (*USART_callBackFuncPtr_t)(void);

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/


USART_errorState_t USART_init(USART_instanceConfig_t *instance);

USART_errorState_t USART_sendByte(u8 UART_NUM, u8 data);

USART_errorState_t USART_recieveByte(u8 UART_NUM, u8 *data);

USART_errorState_t USART_sendBufferAsync(u8 UART_NUM, u8 *buffer, u16 bufLen, USART_callBackFuncPtr_t cb);

USART_errorState_t USART_recieveBufferAsync(u8 UART_NUM, u8 *buffer, u16 bufLen, USART_callBackFuncPtr_t cb);
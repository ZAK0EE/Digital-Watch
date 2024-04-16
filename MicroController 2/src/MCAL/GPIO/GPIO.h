#ifndef GPIO_H_
#define GPIO_H_

/* Define standard types for 8-bit unsigned and signed integers */
typedef unsigned char u8;
typedef signed char s8;

/* Define standard types for 16-bit unsigned and signed integers */
typedef unsigned short int u16;
typedef signed short int s16;

/* Define standard types for 32-bit unsigned and signed integers */
typedef unsigned int u32;
typedef signed int s32;

/* Define a NULL pointer constant */
#define NULLPTR ((void *)0)

/***********************************************************************/

#define GPIO_A ((void*)0x40020000)
#define GPIO_B ((void*)0x40020400)
#define GPIO_C ((void*)0x40020800)
#define GPIO_D ((void*)0x40020C00)
#define GPIO_E ((void*)0x40021000)
#define GPIO_H ((void*)0x40021C00)

/***********************************************************************/

#define GPIO_MODE_INPUT_NOPULL                  0x000UL
#define GPIO_MODE_INPUT_PULLUP                  0x010UL
#define GPIO_MODE_INPUT_PULLDOWN                0x020UL
#define GPIO_MODE_INPUT_ANALOG                  0x003UL

#define GPIO_MODE_OUTPUT_PUSHPULL_NOPULL        0x001UL
#define GPIO_MODE_OUTPUT_PUSHPULL_PULLUP        0x011UL
#define GPIO_MODE_OUTPUT_PUSHPULL_PULLDOWN      0x021UL

#define GPIO_MODE_OUTPUT_OPENDRAIN_NOPULL       0x101UL
#define GPIO_MODE_OUTPUT_OPENDRAIN_PULLUP       0x111UL
#define GPIO_MODE_OUTPUT_OPENDRAIN_PULLDOWN     0x121UL

#define GPIO_MODE_ALTERNATE_PUSHPULL_NOPULL     0x002UL
#define GPIO_MODE_ALTERNATE_PUSHPULL_PULLUP     0x012UL
#define GPIO_MODE_ALTERNATE_PUSHPULL_PULLDOWN   0x022UL

#define GPIO_MODE_ALTERNATE_OPENDRAIN_NOPULL    0x102UL
#define GPIO_MODE_ALTERNATE_OPENDRAIN_PULLUP    0x112UL
#define GPIO_MODE_ALTERNATE_OPENDRAIN_PULLDOWN  0x122UL

/**********************************************************************/

#define GPIO_PIN0               0x00UL
#define GPIO_PIN1               0x01UL
#define GPIO_PIN2               0x02UL
#define GPIO_PIN3               0x03UL
#define GPIO_PIN4               0x04UL
#define GPIO_PIN5               0x05UL
#define GPIO_PIN6               0x06UL
#define GPIO_PIN7               0x07UL
#define GPIO_PIN8               0x08UL
#define GPIO_PIN9               0x09UL
#define GPIO_PIN10              0x0AUL
#define GPIO_PIN11              0x0BUL
#define GPIO_PIN12              0x0CUL
#define GPIO_PIN13              0x0DUL
#define GPIO_PIN14              0x0EUL
#define GPIO_PIN15              0x0FUL

/**********************************************************************/

#define GPIO_LOW_SPEED             0
#define GPIO_MEDIUM_SPEED          1
#define GPIO_HIGH_SPEED            2
#define GPIO_VERY_HIGH_SPEED       3
#define GPIO_NumOfAvailableSpeedConfigs 4

/**********************************************************************/

#define GPIO_SET_STATE           0x01UL
#define GPIO_RESET_STATE         0x00UL

/***********************************************************************/

#define GPIO_AF0                         0x0000UL
#define GPIO_AF1                         0x0001UL
#define GPIO_AF2                         0x0002UL
#define GPIO_AF3                         0x0003UL
#define GPIO_AF4                         0x0004UL
#define GPIO_AF5                         0x0005UL
#define GPIO_AF6                         0x0006UL
#define GPIO_AF7                         0x0007UL
#define GPIO_AF8                         0x0008UL
#define GPIO_AF9                         0x0009UL
#define GPIO_AF10                        0x000AUL
#define GPIO_AF11                        0x000BUL
#define GPIO_AF12                        0x000CUL
#define GPIO_AF13                        0x000DUL
#define GPIO_AF14                        0x000EUL
#define GPIO_AF15                        0x000FUL

/***********************************************************************/


typedef enum{
    GPIO_Ok,
    GPIO_Nok,
    GPIO_InvalidParameter,
    GPIO_NullPtr
}GPIO_errorState_t;

typedef struct{
    void* port;
    u32   pin;
    u32  mode;
    u32 speed;
}GPIO_pinConfig_t;



GPIO_errorState_t GPIO_initPin(GPIO_pinConfig_t *pinStr);
GPIO_errorState_t GPIO_setPin(void* port, u32 pin, u32 state);
GPIO_errorState_t GPIO_getPin(void* port, u32 pin, u32 *pinState);
GPIO_errorState_t GPIO_configAlterFunc(void* port, u32 pin, u32 alterFunc);

#endif
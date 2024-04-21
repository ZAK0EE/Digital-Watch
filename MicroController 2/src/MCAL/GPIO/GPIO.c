#include "GPIO.h"

/********************************************************************/

#define IS_VALID_GPIO_MODE(mode) \
    (((mode) == GPIO_MODE_INPUT_NOPULL) || \
     ((mode) == GPIO_MODE_INPUT_PULLUP) || \
     ((mode) == GPIO_MODE_INPUT_PULLDOWN) || \
     ((mode) == GPIO_MODE_INPUT_ANALOG) || \
     ((mode) == GPIO_MODE_OUTPUT_PUSHPULL_NOPULL) || \
     ((mode) == GPIO_MODE_OUTPUT_PUSHPULL_PULLUP) || \
     ((mode) == GPIO_MODE_OUTPUT_PUSHPULL_PULLDOWN) || \
     ((mode) == GPIO_MODE_OUTPUT_OPENDRAIN_NOPULL) || \
     ((mode) == GPIO_MODE_OUTPUT_OPENDRAIN_PULLUP) || \
     ((mode) == GPIO_MODE_OUTPUT_OPENDRAIN_PULLDOWN) || \
     ((mode) == GPIO_MODE_ALTERNATE_PUSHPULL_NOPULL) || \
     ((mode) == GPIO_MODE_ALTERNATE_PUSHPULL_PULLUP) || \
     ((mode) == GPIO_MODE_ALTERNATE_PUSHPULL_PULLDOWN) || \
     ((mode) == GPIO_MODE_ALTERNATE_OPENDRAIN_NOPULL) || \
     ((mode) == GPIO_MODE_ALTERNATE_OPENDRAIN_PULLUP) || \
     ((mode) == GPIO_MODE_ALTERNATE_OPENDRAIN_PULLDOWN))

/*********************************************************************/
#define IS_VALID_GPIO_PORT(port) \
    (((port) == GPIO_A) || \
     ((port) == GPIO_B) || \
     ((port) == GPIO_C) || \
     ((port) == GPIO_D) || \
     ((port) == GPIO_E) || \
     ((port) == GPIO_H))

/*********************************************************************/

#define IS_VALID_GPIO_PIN(pin) \
    (((pin) >= GPIO_PIN0) && ((pin) <= GPIO_PIN15))

/*********************************************************************/

#define IS_VALID_GPIO_STATE(state) \
    (((state) == GPIO_SET_STATE) || \
     ((state) == GPIO_RESET_STATE))

/*********************************************************************/

#define IS_VALID_GPIO_SPEED(speed) \
    (((speed) >= GPIO_LOW_SPEED) && ((speed) <= GPIO_VERY_HIGH_SPEED))

/*********************************************************************/

#define IS_VALID_GPIO_AF(af) \
    (((af) >= GPIO_AF0) && ((af) <= GPIO_AF15))

/*********************************************************************/

#define MASK_1_BITS         0x01UL
#define MASK_2_BITS         0x03UL
#define MASK_4_BITS         0x0FUL


#define GET_MODE_FROM_CONFIG_MASK(MASK)             (MASK & 0x0FUL)
#define GET_PULLING_FROM_CONFIG_MASK(MASK)          ((MASK & 0x0F0UL) >> 4)
#define GET_OUTPUT_TYPE_FROM_CONFIG_MASK(MASK)      ((MASK & 0x0F00UL) >> 8)
/*********************************************************************/

typedef struct{
    volatile u32 MODER;        // GPIO port mode register
    volatile u32 OTYPER;       // GPIO port output type register
    volatile u32 OSPEEDR;      // GPIO port output speed register
    volatile u32 PUPDR;        // GPIO port pull-up/pull-down register
    volatile u32 IDR;          // GPIO port input data register
    volatile u32 ODR;          // GPIO port output data register
    volatile u32 BSRR;         // GPIO port bit set/reset register
    volatile u32 LCKR;         // GPIO port configuration lock register
    volatile u32 AFRL;         // GPIO alternate function low register
    volatile u32 AFRH;         // GPIO alternate function high register
}GPIO_regs_t;

/*********************************************************************/



GPIO_errorState_t GPIO_initPin(GPIO_pinConfig_t *pinConfig)
{
    GPIO_errorState_t Loc_errorState = GPIO_Nok;
    volatile GPIO_regs_t* const GPIO = (volatile GPIO_regs_t*)pinConfig->port;

    if(pinConfig == NULLPTR)
    {
        Loc_errorState = GPIO_NullPtr;
    }
    else if(!IS_VALID_GPIO_PORT(pinConfig->port))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else if(!IS_VALID_GPIO_PIN(pinConfig->pin))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else if(!IS_VALID_GPIO_MODE(pinConfig->mode))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else if(!IS_VALID_GPIO_SPEED(pinConfig->speed))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else
    {
        Loc_errorState = GPIO_Ok;
        u32 Loc_mode = GET_MODE_FROM_CONFIG_MASK(pinConfig->mode);
        u32 Loc_pulling = GET_PULLING_FROM_CONFIG_MASK(pinConfig->mode);
        u32 Loc_outputType = GET_OUTPUT_TYPE_FROM_CONFIG_MASK(pinConfig->mode);

        GPIO->MODER    = ( (GPIO->MODER   & ~(MASK_2_BITS << (pinConfig->pin * 2))) | (Loc_mode         << (pinConfig->pin * 2)) );
        GPIO->PUPDR    = ( (GPIO->PUPDR   & ~(MASK_2_BITS << (pinConfig->pin * 2))) | (Loc_pulling      << (pinConfig->pin * 2)) );
        GPIO->OTYPER   = ( (GPIO->OTYPER  & ~(MASK_1_BITS << (pinConfig->pin)    )) | (Loc_outputType   << (pinConfig->pin)    ) );
        GPIO->OSPEEDR  = ( (GPIO->OSPEEDR & ~(MASK_2_BITS << (pinConfig->pin * 2))) | (pinConfig->speed << (pinConfig->pin * 2)) );
    }

    return Loc_errorState;
}

GPIO_errorState_t GPIO_setPin(void* port, u32 pin, u32 state)
{
    GPIO_errorState_t Loc_errorState = GPIO_Nok;
    volatile GPIO_regs_t* const GPIO = (volatile GPIO_regs_t*)port;

    if(port == NULLPTR)
    {
        Loc_errorState = GPIO_NullPtr;
    }
    else if(!IS_VALID_GPIO_PORT(port))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else if(!IS_VALID_GPIO_PIN(pin))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else if(!IS_VALID_GPIO_STATE(state))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else
    {
        Loc_errorState = GPIO_Ok;
        GPIO->BSRR = MASK_1_BITS<<(pin + (!state * 16));
    }

    return Loc_errorState;
}

GPIO_errorState_t GPIO_getPin(void* port, u32 pin, u32 *pinState)
{
    GPIO_errorState_t Loc_errorState = GPIO_Nok;
    volatile GPIO_regs_t* const GPIO = (volatile GPIO_regs_t*)port;

    if(port == NULLPTR)
    {
        Loc_errorState = GPIO_NullPtr;
    }
    else if(!IS_VALID_GPIO_PORT(port))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else if(!IS_VALID_GPIO_PIN(pin))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else
    {
        Loc_errorState = GPIO_Ok;
        *pinState = (GPIO->IDR >> pin) & MASK_1_BITS;
    }
    return Loc_errorState;
}

GPIO_errorState_t GPIO_configAlterFunc(void* port, u32 pin, u32 alterFunc)
{
    GPIO_errorState_t Loc_errorState = GPIO_Nok;
    volatile GPIO_regs_t* const GPIO = (volatile GPIO_regs_t*)port;

    if(port == NULLPTR)
    {
        Loc_errorState = GPIO_NullPtr;
    }
    else if((!IS_VALID_GPIO_PORT(port)) || (!IS_VALID_GPIO_PIN(pin)) || (!IS_VALID_GPIO_AF(alterFunc)))
    {
        Loc_errorState = GPIO_InvalidParameter;
    }
    else
    {
        Loc_errorState = GPIO_Ok;
        if(pin <= 7)
        {
            GPIO->AFRL = (((GPIO->AFRL) & ~(MASK_4_BITS << 4*pin)) | (alterFunc << 4*pin));
        }
        else if(pin > 7)
        {
            GPIO->AFRH = (((GPIO->AFRH) & ~(MASK_4_BITS << 4*(pin-8))) | (alterFunc << 4*(pin-8)));
        }
    }
    return Loc_errorState;
}
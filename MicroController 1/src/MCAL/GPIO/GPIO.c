/**
 * @file GPIO.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief Implementation of the GPIO interface
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/GPIO/GPIO.h"
#include "assertparam.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define MASK_1BIT  (0x1UL)
#define MASK_2BITS (0X3UL)

#define GPIO_PINMODE_GET_MODE(PinMode) (PinMode & 0x00FUL)
#define GPIO_PINMODE_GET_PULL(PinMode) ((PinMode & 0x0F0UL) >> 4)
#define GPIO_PINMODE_GET_OUTPUT_TYPE(PinMode) ((PinMode & 0xF00UL) >> 8)


/************************************/
/***************Registers************/
/************************************/
#define GPIOA_BASE (0x40020000UL)
#define GPIOB_BASE (0x40020400UL)
#define GPIOC_BASE (0x40020800UL)
#define GPIOD_BASE (0x40020C00UL)
#define GPIOE_BASE (0x40021000UL)
#define GPIOH_BASE (0x40021C00UL)

#define NUM_OF_GPIOS (6)

/************************************/
/***************Validators************/
/************************************/
#define IS_GPIO_PORT(PORT) (((PORT) == GPIO_GPIOA) || \
                            ((PORT) == GPIO_GPIOB) || \
                            ((PORT) == GPIO_GPIOC) || \
                            ((PORT) == GPIO_GPIOD) || \
                            ((PORT) == GPIO_GPIOE) || \
                            ((PORT) == GPIO_GPIOH))

#define IS_GPIO_PIN(PIN) (((PIN) >= GPIO_PIN0) && ((PIN) <= GPIO_PIN15))

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_INPUT_NOPULL)           || \
                            ((MODE) == GPIO_MODE_INPUT_PULLUP)           || \
                            ((MODE) == GPIO_MODE_INPUT_PULLDOWN)         || \
                            ((MODE) == GPIO_MODE_INPUT_ANALOG)           || \
                            ((MODE) == GPIO_MODE_OUTPUT_PUSHPULL_NOPULL) || \
                            ((MODE) == GPIO_MODE_OUTPUT_PUSHPULL_PULLUP) || \
                            ((MODE) == GPIO_MODE_OUTPUT_PUSHPULL_PULLDOWN) || \
                            ((MODE) == GPIO_MODE_OUTPUT_OPENDRAIN_NOPULL) || \
                            ((MODE) == GPIO_MODE_OUTPUT_OPENDRAIN_PULLUP) || \
                            ((MODE) == GPIO_MODE_OUTPUT_OPENDRAIN_PULLDOWN) || \
                            ((MODE) == GPIO_MODE_ALTERNATE_PUSHPULL_NOPULL) || \
                            ((MODE) == GPIO_MODE_ALTERNATE_PUSHPULL_PULLUP) || \
                            ((MODE) == GPIO_MODE_ALTERNATE_PUSHPULL_PULLDOWN) || \
                            ((MODE) == GPIO_MODE_ALTERNATE_OPENDRAIN_NOPULL) || \
                            ((MODE) == GPIO_MODE_ALTERNATE_OPENDRAIN_PULLUP) || \
                            ((MODE) == GPIO_MODE_ALTERNATE_OPENDRAIN_PULLDOWN))

#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_SPEED_LOW)       || \
                              ((SPEED) == GPIO_SPEED_MEDIUM)    || \
                              ((SPEED) == GPIO_SPEED_HIGH)      || \
                              ((SPEED) == GPIO_SPEED_VERY_HIGH))

#define IS_GPIO_PIN_STATE(STATE) (((STATE) == GPIO_PINSTATE_RESET) || \
                                  ((STATE) == GPIO_PINSTATE_SET))

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/***********************************************************/
/******************Peripheral Register GPIO******************/
/***********************************************************/

/**
 * @brief Structure representing the GPIO peripheral registers.
 *
 * This structure encapsulates the various registers associated with a GPIO port.
 */
typedef struct {
    uint32_t MODER;     /**< GPIO port mode register. */
    uint32_t OTYPER;    /**< GPIO port output type register. */
    uint32_t OSPEEDR;   /**< GPIO port output speed register. */
    uint32_t PUPDR;     /**< GPIO port pull-up/pull-down register. */
    uint32_t IDR;       /**< GPIO port input data register. */
    uint32_t ODR;       /**< GPIO port output data register. */
    uint32_t BSRR;      /**< GPIO port bit set/reset register. */
    uint32_t LCKR;      /**< GPIO port lock register. */
    uint32_t AFRL;      /**< GPIO port alternate function low register. */
    uint32_t AFRH;      /**< GPIO port alternate function high register. */
} GPIO_TypeDef;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

GPIO_TypeDef volatile *const GPIOS[NUM_OF_GPIOS] = {
    [GPIO_GPIOA]=(GPIO_TypeDef volatile *const)GPIOA_BASE,
    [GPIO_GPIOB]=(GPIO_TypeDef volatile *const)GPIOB_BASE,
    [GPIO_GPIOC]=(GPIO_TypeDef volatile *const)GPIOC_BASE,
    [GPIO_GPIOD]=(GPIO_TypeDef volatile *const)GPIOD_BASE,
    [GPIO_GPIOE]=(GPIO_TypeDef volatile *const)GPIOE_BASE,
    [GPIO_GPIOH]=(GPIO_TypeDef volatile *const)GPIOH_BASE,
};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

MCAL_Status_t GPIO_initPin(GPIO_PinConfig_t const *PinConfig)
{
    GPIO_TypeDef volatile *const GPIO = GPIOS[PinConfig->Port];

    assert_param(PinConfig);
    assert_param(IS_GPIO_PORT(PinConfig->Port));
    assert_param(IS_GPIO_PIN(PinConfig->PinNumber));
    assert_param(IS_GPIO_SPEED(PinConfig->PinSpeed));
    assert_param(IS_GPIO_MODE(PinConfig->PinMode));

    /* Decoding the PinMode */
    uint32_t PinMode = GPIO_PINMODE_GET_MODE(PinConfig->PinMode);
    uint32_t PinPull = GPIO_PINMODE_GET_PULL(PinConfig->PinMode);
    uint32_t PinOutputType = GPIO_PINMODE_GET_OUTPUT_TYPE(PinConfig->PinMode);


    /* Set Pin Mode */
    GPIO->MODER = (GPIO->MODER & ~(MASK_2BITS << (PinConfig->PinNumber * 2))) | (PinMode << (PinConfig->PinNumber * 2));

    GPIO->PUPDR = (GPIO->PUPDR & ~(MASK_2BITS << (PinConfig->PinNumber * 2))) | (PinPull << (PinConfig->PinNumber * 2));

    GPIO->OTYPER = (GPIO->OTYPER & ~(MASK_1BIT << PinConfig->PinNumber)) | (PinOutputType << PinConfig->PinNumber);

    /* Set Pin Speed */
    GPIO->OSPEEDR = (GPIO->OSPEEDR & ~(MASK_2BITS << (PinConfig->PinNumber * 2))) | (PinConfig->PinSpeed << (PinConfig->PinNumber * 2));

    return MCAL_OK;
}

MCAL_Status_t GPIO_setPinValue(GPIO_Port_t Port, GPIO_Pin_t PinNumber, GPIO_PinState_t PinState)
{
    assert_param(IS_GPIO_PORT(Port));
    assert_param(IS_GPIO_PIN(PinNumber));
    assert_param(IS_GPIO_PIN_STATE(PinState));

    GPIO_TypeDef volatile *const GPIO = GPIOS[Port];
    GPIO->ODR = (GPIO->ODR & ~(MASK_1BIT << PinNumber)) | (PinState << PinNumber);
    return MCAL_OK;
}

GPIO_PinState_t GPIO_getPinValue(GPIO_Port_t Port, GPIO_Pin_t PinNumber)
{

    assert_param(IS_GPIO_PORT(Port));
    assert_param(IS_GPIO_PIN(PinNumber));
    
    GPIO_TypeDef volatile *const GPIO = GPIOS[Port];

    uint32_t PinValue = (GPIO->IDR >> PinNumber) & MASK_1BIT;

    return PinValue;
}

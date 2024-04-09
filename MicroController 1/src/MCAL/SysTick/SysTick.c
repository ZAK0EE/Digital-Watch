/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "SysTick.h"
#include "assertparam.h"
#include <stddef.h>
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/************************************/
/***************Registers************/
/************************************/

#define SYSTICK_BASE     (0xE000E010UL)

#define SYSTICK    ((SysTick_t volatile* const)(SYSTICK_BASE))

#define SYSTICK_CTRL_CLKSOURCE_MASK (0x4UL)
#define SYSTICK_CTRL_TICKINT_MASK (0X2UL)
#define SYSTICK_CTRL_ENABLE_MASK (0X1UL)


/************************************/
/***************Validators***********/
/************************************/

/**
 * @brief Macro to validate the SysTick clock source enumeration.
 *
 * This macro checks if the given value is a valid SysTick clock source enumeration.
 *
 * @param source The SysTick clock source enumeration value to validate.
 * @retval 1 if the value is a valid SysTick clock source enumeration, 0 otherwise.
 */
#define IS_VALID_SYSTICK_CLOCK_SOURCE(source) \
    (((source) == SYSTICK_CLK_AHB_DIV_8) || \
     ((source) == SYSTICK_CLK_AHB))

/**
 * @brief Macro to validate the SysTick exception state enumeration.
 *
 * This macro checks if the given value is a valid SysTick exception state enumeration.
 *
 * @param state The SysTick exception state enumeration value to validate.
 * @retval 1 if the value is a valid SysTick exception state enumeration, 0 otherwise.
 */
#define IS_VALID_SYSTICK_EXCEPTION_STATE(state) \
    (((state) == SYSTICK_EXCEPTION_ENABLED) || \
     ((state) == SYSTICK_EXCEPTION_DISABLED))

/**
 * @brief Macro to validate the SysTick ticks.
 * This macro checks if the value of ticks is less than or equal to the maximum 23-bit value 
 */
#define IS_VALID_TICK(ticks) ((ticks) <= 0x7FFFFF)


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
/**
 * @brief Structure representing the SysTick timer registers.
 *
 */
typedef struct 
{
    uint32_t CTRL;  /**< Control and status register. */
    uint32_t LOAD;  /**< Reload value register. */
    uint32_t VAL;   /**< Current value register. */
    uint32_t CALIB; /**< Calibration value register. */
} SysTick_t;



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static SysTick_CallBackFn_t callBackFunction = NULL;


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/


/**
 * @brief A functions that stops the SysTick timer
 */
static void stopSysTick()
{
    SYSTICK->CTRL &= ~(SYSTICK_CTRL_ENABLE_MASK);
}


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void SysTick_init(SysTick_Config_t const *config)
{
    assert_param(IS_VALID_SYSTICK_CLOCK_SOURCE(config->ClockSource));
    assert_param(IS_VALID_SYSTICK_EXCEPTION_STATE(config->ExceptionState));

    uint32_t CTRL = SYSTICK->CTRL;
    CTRL = (CTRL & SYSTICK_CTRL_CLKSOURCE_MASK) | config->ClockSource;
    CTRL = (CTRL & SYSTICK_CTRL_TICKINT_MASK) | config->ExceptionState;

    SYSTICK->CTRL = CTRL;

    callBackFunction = config->CallbackFunction;
}

void SysTick_startTimerMS(uint32_t timeMS)
{

    stopSysTick();

    uint64_t freq = (SYSTICK->CTRL & SYSTICK_CTRL_CLKSOURCE_MASK) ? SYSTICK_AHB_CLK : SYSTICK_AHB_CLK / 8;
    SYSTICK->LOAD = ((freq / 1000) * (timeMS)) - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL |= SYSTICK_CTRL_ENABLE_MASK;
}
void SysTick_startTickCounter(uint32_t ticks)
{
    assert_param(IS_VALID_TICK(ticks));

    stopSysTick();
    SYSTICK->LOAD = ticks;
    SYSTICK->CTRL |= SYSTICK_CTRL_ENABLE_MASK;
}

uint32_t SysTick_currentTick(void)
{
    uint32_t currentTick = SYSTICK->VAL;
    return currentTick;
}

void SysTick_stop(void)
{
    stopSysTick();
}

void SysTick_Handler(void)
{
    
    if(callBackFunction != NULL )
        callBackFunction();

}
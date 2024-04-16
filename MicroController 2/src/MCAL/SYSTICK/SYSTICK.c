/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "SYSTICK.h"
#include "SYSTICK_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/


#define SYSTICK_BASE_ADDRESS        (u32)0xE000E010

#define SYSTICK_COUNT_FLAG_MASK     (u32)0x10000
#define SYSTICK_TICKINT_MASK        (u32)0x2
#define SYSTICK_ENABLE_MASK         (u32)0x1
#define SYSTICK_MAX_RELOAD_VALUE    (u64)0x00FFFFFF

#define IS_SYSTICK_RUN_MODE(mode) (((mode) == SYSTICK_RUN_ONE_TIME) || ((mode) == SYSTICK_RUN_PERIODIC))

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct{
    volatile u32 CTRL;     // SysTick Control and Status Register
    volatile u32 LOAD;     // SysTick Reload Value Register
    volatile u32 VAL;           // SysTick Current Value Register
    volatile const u32 CALIB;   // SysTick Calibration Value Register
}SYSTICK_regs_t;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

static u32 systickClockSrc = SYSTICK_CLOCK_SOURCE;
static u32 systickRunMode  = SYSTICK_RUN_PERIODIC;
static SYSTICK_callBackFunPtr_t systick_CBFs[SYSTICK_MAX_SUPPORTED_CBF] = {NULLPTR};

volatile SYSTICK_regs_t* const SYSTICK = (volatile SYSTICK_regs_t*)SYSTICK_BASE_ADDRESS;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


SYSTICK_errorState_t SYSTICK_SelectClockSource(u32 clkSrc)
{
    SYSTICK_errorState_t errorState = SYSTICK_Nok;

    if(clkSrc != SYSTICK_CLOCK_SOURCE_AHB && clkSrc != SYSTICK_CLOCK_SOURCE_AHB_DIV8)
    {
        errorState = SYSTICK_InvalidParameter;
    }
    else
    {
        errorState = SYSTICK_Ok;
        systickClockSrc = clkSrc;
    }
    return errorState;
}

SYSTICK_errorState_t SYSTICK_StartCounting(u32 mode)
{
    SYSTICK_errorState_t errorState = SYSTICK_Nok;
    
    if(!IS_SYSTICK_RUN_MODE(mode))
    {
        errorState = SYSTICK_InvalidParameter;
    }
    else
    {
        errorState = SYSTICK_Ok;
        systickRunMode = mode;
        SYSTICK->VAL = 0;
        SYSTICK->CTRL = systickClockSrc;
    }
    return errorState;
}

void SYSTICK_StopCounting(void)
{
    SYSTICK->CTRL = 0;
}


SYSTICK_errorState_t SYSTICK_SetTime_ms(u32 time_ms)
{
    SYSTICK_errorState_t errorState = SYSTICK_Nok;
    u64 loadValue = ((time_ms*SYSTICK_CLOCK_VALUE)/1000) - 1;

    if(loadValue > SYSTICK_MAX_RELOAD_VALUE)
    {
        errorState = SYSTICK_InvalidTimeValue;
    }
    else
    {
        errorState = SYSTICK_Ok;
        SYSTICK->LOAD = loadValue;
    }
    return errorState;
}

void SysTick_Handler(void)
{
    u8 iter = 0;
    for(iter = 0; iter < SYSTICK_MAX_SUPPORTED_CBF; iter++)
    {
        if(systick_CBFs[iter])
        {
            systick_CBFs[iter]();
        }
        if(systickRunMode == SYSTICK_RUN_ONE_TIME)
        {
            SYSTICK->CTRL = 0;
        }
    }
}

SYSTICK_errorState_t SYSTICK_SetCallBack(SYSTICK_callBackFunPtr_t callBackPtr, u8 index)
{
    SYSTICK_errorState_t errorState = SYSTICK_Nok;

    if(callBackPtr == NULLPTR)
    {
        errorState = SYSTICK_NullPtr;
    }
    else if(index >= SYSTICK_MAX_SUPPORTED_CBF)
    {
        errorState = SYSTICK_InvalidParameter;
    }
    else
    {
        errorState = SYSTICK_Ok;
        systick_CBFs[index] = callBackPtr;
    }
    return errorState;
}

void SYSTICK_IsExpired(void);


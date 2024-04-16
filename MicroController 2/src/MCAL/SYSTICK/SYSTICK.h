#ifndef SYSTICK_H_
#define SYSTICK_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/* Define standard types for 8-bit unsigned and signed integers */
typedef unsigned char u8;
typedef signed char s8;

/* Define standard types for 16-bit unsigned and signed integers */
typedef unsigned short int u16;
typedef signed short int s16;

/* Define standard types for 32-bit unsigned and signed integers */
typedef unsigned int u32;
typedef signed int s32;

/* Define standard types for 64-bit unsigned and signed integers */
typedef unsigned long long int u64;
typedef signed long long int s64;


/* Define a NULL pointer constant */
#define NULLPTR ((void *)0)


#define SYSTICK_RUN_ONE_TIME  (u32)0x50
#define SYSTICK_RUN_PERIODIC  (u32)0x60

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



typedef void (*SYSTICK_callBackFunPtr_t)(void);

typedef enum{
    SYSTICK_Ok,
    SYSTICK_Nok,
    SYSTICK_NullPtr,
    SYSTICK_InvalidParameter,
    SYSTICK_InvalidTimeValue
}SYSTICK_errorState_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

SYSTICK_errorState_t SYSTICK_SelectClockSource(u32 clkSrc);
SYSTICK_errorState_t SYSTICK_StartCounting(u32 mode);
void SYSTICK_StopCounting(void);

SYSTICK_errorState_t SYSTICK_SetTime_ms(u32 time_ms);
SYSTICK_errorState_t SYSTICK_SetCallBack(SYSTICK_callBackFunPtr_t callBackPtr, u8 index);

void SYSTICK_IsExpired(void);



#endif // SYSTICK_H_
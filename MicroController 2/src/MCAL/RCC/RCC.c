#include "MCAL/RCC/RCC.h"

/*Target Base Address*/
#define RCC_BASE_ADDRESS 0x40023800

/*Clear System Clock Mask*/
#define RCC_CLEAR_SYSTEM_CLOCK_MASK 0xFFFFFFFC

/*Clear PLL Configurations Mask*/
#define RCC_CLEAR_PLL_CONFG_MASK 0xFFFC8000

/*Set Input Clock to PLL Mask*/
#define RCC_PLL_INPUT_CLOCK_MASK 0x00400000

/*Clear AHB PreScaler Mask*/
#define RCC_CLEAR_AHB_PRESCALER_MASK  0xFFFFFF0F

/*Clear APB1 PreScaler Mask*/
#define RCC_CLEAR_APB2_PRESCALER_MASK 0xFFFF1FFF


#define RCC ((volatile RCC_Registers_t * const)RCC_BASE_ADDRESS)

/*RCC Registers*/
typedef struct
{

     u32 CR;
     u32 PLLCFGR;
     u32 CFGR;
     u32 CIR;
     u32 AHB1RSTR;
     u32 AHB2RSTR;
     u32 RESERVED0;
     u32 RESERVED1;
     u32 APB1RSTR;
     u32 APB2RSTR;
     u32 RESERVED2;
     u32 RESERVED3;
     u32 AHB1ENR;
     u32 AHB2ENR;
     u32 RESERVED4;
     u32 RESERVED5;
     u32 APB1ENR;
     u32 APB2ENR;
     u32 RESERVED6;
     u32 RESERVED7;
     u32 AHB1LPENR;
     u32 AHB2LPENR;
     u32 RESERVED8;
     u32 RESERVED9;
     u32 APB1LPENR;
     u32 APB2LPENR;
     u32 RESERVED10;
     u32 RESERVED11;
     u32 BDCR;
     u32 CSR;
     u32 RESERVED12;
     u32 RESERVED13;
     u32 SSCGR;
     u32 PLLI2SCFGR;
     u32 RESERVED14;
     u32 DCKCFGR;

}RCC_Registers_t;


/*STATIC FUNCTIONS APIs*/

static Error_t RCC_Validate_Enable_Clock (u32 RCC_SYSTEM_CLOCK);        /*Validate the system clock name*/
static Error_t RCC_Validate_Main_System_Clock (u32 RCC_SYSTEM_CLOCK);   /*Validate the main system clock name*/
static Error_t RCC_Validate_PLL_Input_Clock (u32 RCC_SYSTEM_CLOCK);     /*Validate the PLL input clock name*/
static Error_t RCC_Validate_APB2_PreScaler (u32 PreScale_Value);        /*Validate the range of APB2 PreScaler*/
static Error_t RCC_Validate_AHB_PreScaler (u32 PreScaler_Value);        /*Validate the range of AHB PreScaler*/
static Error_t RCC_Validate_P_Factor (u32 PLL_P_Factor);                /*Validate the range of P factor of PLL*/
static Error_t RCC_Validate_N_Factor (u32 PLL_N_Factor);                /*Validate the range of N factor of PLL*/
static Error_t RCC_Validate_M_Factor (u32 PLL_M_Factor);                /*Validate the range of M factor of PLL*/
static u32 RCC_APB2_PreScaler_Mask (u32 PreScaler_Value);               /*Generate Masks for every APB2 Prescaler Value*/
static u32 RCC_AHB_PreScaler_Mask (u32 PreScaler_Value);                /*Generate Masks for every AHB Prescaler Value*/
static u32 RCC_PLL_P_Mask (u32 PLL_P_Factor);                           /*Generate Masks for every P factor Value*/
static u32 RCC_PLL_N_Mask (u32 PLL_N_Factor);                           /*Generate Masks for every N factor Value*/
static u32 RCC_PLL_M_Mask (u32 PLL_M_Factor);                           /*Generate Masks for every M factor Value*/


Error_t RCC_Enable_Clock (u32 RCC_ENABLE_SYSTEM_CLOCK)
{
    Error_t loc_Error;
    loc_Error = RCC_Validate_Enable_Clock(RCC_ENABLE_SYSTEM_CLOCK);
    
    if (loc_Error != ERROR_Ok) {/*RETURN*/}
    else
    {

    RCC->CR |= RCC_ENABLE_SYSTEM_CLOCK;

    }
    return loc_Error;
}

Error_t RCC_Disable_Clock (u32 RCC_ENABLE_SYSTEM_CLOCK)
{
    Error_t loc_Error = RCC_Validate_Enable_Clock(RCC_ENABLE_SYSTEM_CLOCK);

    if (loc_Error != ERROR_Ok) {/*RETRUN*/}
    else
    {
        
        RCC->CR &= ~RCC_ENABLE_SYSTEM_CLOCK;

    }
    return loc_Error;
}

Error_t RCC_Confg_PLL (u32 PLL_P_Factor, u32 PLL_N_Factor, u32 PLL_M_Factor)
{
    u32 loc_Temp = RCC->PLLCFGR;
    
    loc_Temp &= RCC_CLEAR_PLL_CONFG_MASK;

    Error_t loc_Error = ERROR_Ok;

    if ( (loc_Error = RCC_Validate_P_Factor(PLL_P_Factor)) != ERROR_Ok){/*RETURN*/}
    else
    if ( (loc_Error = RCC_Validate_N_Factor(PLL_N_Factor)) != ERROR_Ok ){/*RETURN*/}
    else
    if ( (loc_Error = RCC_Validate_M_Factor(PLL_M_Factor)) != ERROR_Ok){/*RETURN*/}
    else
    {
        loc_Temp |= RCC_PLL_P_Mask(PLL_P_Factor); 
        loc_Temp |= RCC_PLL_N_Mask(PLL_N_Factor);
        loc_Temp |= RCC_PLL_M_Mask(PLL_M_Factor);


        RCC->PLLCFGR = loc_Temp;
    }

    return loc_Error;    
}

Error_t RCC_Set_Main_System_Clock (u32 RCC_SYSTEM_CLOCK)
{
    u32 loc_Temp = RCC->CFGR;
    loc_Temp &= RCC_CLEAR_SYSTEM_CLOCK_MASK;
    
    Error_t loc_Error = RCC_Validate_Main_System_Clock(RCC_SYSTEM_CLOCK);

    if (loc_Error != ERROR_Ok) {/*RETURN*/}
    else
    {
        loc_Temp |= RCC_SYSTEM_CLOCK;
        RCC->CFGR = loc_Temp;
    }

    return loc_Error;
}
 
Error_t RCC_Set_PLL_Input_Clock (u32 RCC_SYSTEM_CLOCK)
{
    Error_t loc_Error = RCC_Validate_PLL_Input_Clock(RCC_SYSTEM_CLOCK);
    
    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        switch (RCC_SYSTEM_CLOCK)
        {
        case RCC_SYSTEM_CLOCK_HSI: RCC->PLLCFGR &= ~RCC_PLL_INPUT_CLOCK_MASK;
            break;
        case RCC_SYSTEM_CLOCK_HSE: RCC->PLLCFGR |= RCC_PLL_INPUT_CLOCK_MASK;
            break;
        
        default: loc_Error = ERROR_Invalid_Argument;
            break;
        }
    }
    return loc_Error;
}

Error_t RCC_Set_APB2_PreScaler (u32 PreScaler_Value)
{
    u32 loc_Temp = RCC->CFGR;
    loc_Temp &= RCC_CLEAR_APB2_PRESCALER_MASK;

    Error_t loc_Error = RCC_Validate_APB2_PreScaler(PreScaler_Value);

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        loc_Temp |= RCC_APB2_PreScaler_Mask(PreScaler_Value);
        RCC->CFGR = loc_Temp;
    }

    return loc_Error;
} 

Error_t RCC_Set_AHB_PreScaler (u32 PreScaler_Value)
{
    u32 loc_Temp = RCC->CFGR;
    loc_Temp &= RCC_CLEAR_AHB_PRESCALER_MASK; 

    u32 loc_Error = RCC_Validate_AHB_PreScaler(PreScaler_Value);

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        loc_Temp |= RCC_AHB_PreScaler_Mask(PreScaler_Value);
        RCC->CFGR = loc_Temp; 
    }

    return loc_Error;

}

Error_t RCC_Enable_AHB1_Peripheral_Clock (u32 Peripheral)
{
    Error_t loc_Error = ERROR_Ok;

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        RCC->AHB1ENR |= Peripheral;
    }
/*Added | in above line to test*/
    return loc_Error;
}

Error_t RCC_Disable_AHB1_Peripheral_Clock (u32 Peripheral)
{
    u32 loc_Error = ERROR_Ok;

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        RCC->AHB1ENR &= ~Peripheral;
    }

    return loc_Error;
}

Error_t RCC_Enable_APB1_Peripheral_Clock (u32 Peripheral)
{
    u32 loc_Error = ERROR_Ok;

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        RCC->APB1ENR |= Peripheral;
    }

    return loc_Error;
}

Error_t RCC_Disable_APB1_Peripheral_Clock (u32 Peripheral)
{
    u32 loc_Error = ERROR_Ok;

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        RCC->APB1ENR= ~Peripheral;
    }

    return loc_Error;
}

Error_t RCC_Enable_APB2_Peripheral_Clock (u32 Peripheral)
{
    u32 loc_Error = ERROR_Ok;

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        RCC->APB2ENR |= Peripheral;
    }

    return loc_Error;
}

Error_t RCC_Disable_APB2_Peripheral_Clock (u32 Peripheral)
{
    u32 loc_Error = ERROR_Ok;

    if (loc_Error != ERROR_Ok){/*RETURN*/}
    else
    {
        RCC->APB2ENR= ~Peripheral;
    }

    return loc_Error;
}


/**********************************************************HELPERS FUNCTIONS**********************************************************/

static Error_t RCC_Validate_Enable_Clock (u32 RCC_ENABLE_SYSTEM_CLOCK)
{
    Error_t loc_Error;

    switch (RCC_ENABLE_SYSTEM_CLOCK)
    {
    case RCC_ENABLE_SYSTEM_CLOCK_HSI: loc_Error = ERROR_Ok;
        break;
    case RCC_ENABLE_SYSTEM_CLOCK_HSE: loc_Error = ERROR_Ok;
        break;
    case RCC_ENABLE_SYSTEM_CLOCK_PLL: loc_Error = ERROR_Ok;
        break;
    
    default: loc_Error = ERROR_Invalid_Argument;
        break;
    }

    return loc_Error;
}

static Error_t RCC_Validate_Main_System_Clock (u32 RCC_SYSTEM_CLOCK)
{
    Error_t loc_Error = ERROR_Ok;

    switch (RCC_SYSTEM_CLOCK)
    {
    case RCC_SYSTEM_CLOCK_HSI: loc_Error = ERROR_Ok;
        break;
    case RCC_SYSTEM_CLOCK_HSE: loc_Error = ERROR_Ok;
        break;
    case RCC_SYSTEM_CLOCK_PLL: loc_Error = ERROR_Ok;
        break;
    
    default: loc_Error = ERROR_Invalid_Argument;
        break;
    }

    return loc_Error;
}

static Error_t RCC_Validate_PLL_Input_Clock (u32 RCC_SYSTEM_CLOCK)
{
    Error_t loc_Error = ERROR_Ok;
    switch (RCC_SYSTEM_CLOCK)
    {
    case RCC_SYSTEM_CLOCK_HSI: loc_Error = ERROR_Ok;
        break;
    case RCC_SYSTEM_CLOCK_HSE: loc_Error = ERROR_Ok;
        break;
    
    default: loc_Error = ERROR_Invalid_Argument;
        break;
    }
    return loc_Error;
}

static Error_t RCC_Validate_P_Factor (u32 PLL_P_Factor)
{ 
    Error_t loc_Error = ERROR_Nok;

    switch (PLL_P_Factor)
    {
    case 2: loc_Error = ERROR_Ok;
        break;
    case 4: loc_Error = ERROR_Ok;
        break;
    case 6: loc_Error = ERROR_Ok;
        break;
    case 8: loc_Error = ERROR_Ok;
        break;
    default: loc_Error = Error_Out_Of_Range;
        break;
    }

    return loc_Error;
}

static Error_t RCC_Validate_N_Factor (u32 PLL_N_Factor)
{
    Error_t loc_Error = ERROR_Nok;

    if ( (PLL_N_Factor >= 192) || (PLL_N_Factor <= 432) )
    {
        loc_Error = ERROR_Ok;
    }
    else
    {
        loc_Error = Error_Out_Of_Range;
    }

    return loc_Error;
}

static Error_t RCC_Validate_M_Factor (u32 PLL_M_Factor)
{
    Error_t loc_Error = ERROR_Nok;

    if (PLL_M_Factor >= 2 || PLL_M_Factor <= 63 )
    {
        loc_Error = ERROR_Ok;
    }
    else
    {
        loc_Error = Error_Out_Of_Range;
    }
    
    return loc_Error;
}

static Error_t RCC_Validate_APB2_PreScaler (u32 PreScale_Value)
{
    Error_t loc_Error = ERROR_Ok;

    switch (PreScale_Value)
    {
    case 2: loc_Error = ERROR_Ok;
        break;
    case 4: loc_Error = ERROR_Ok;
        break;
    case 8: loc_Error = ERROR_Ok;
        break;
    case 16: loc_Error = ERROR_Ok;
        break;
    
    default: loc_Error = ERROR_Invalid_Argument;
        break;
    }
    return loc_Error;
}

static Error_t RCC_Validate_AHB_PreScaler (u32 PreScaler_Value)
{
    Error_t loc_Error = ERROR_Nok;

    switch (PreScaler_Value)
    {
    case 1: loc_Error = ERROR_Ok;
        break;
    case 2: loc_Error = ERROR_Ok;
        break;
    case 4: loc_Error = ERROR_Ok;
        break;
    case 8: loc_Error = ERROR_Ok;
        break;
    case 16: loc_Error = ERROR_Ok;
        break;
    case 64: loc_Error = ERROR_Ok;
        break;
    case 128: loc_Error = ERROR_Ok;
        break;
    case 256: loc_Error = ERROR_Ok;
        break;
    case 512: loc_Error = ERROR_Ok;
        break;
    
    default: loc_Error = Error_Out_Of_Range;
        break;
    }

    return loc_Error;
}

static u32 RCC_PLL_P_Mask (u32 PLL_P_Factor)
{
    return ( ((PLL_P_Factor/2) - 1) );
}

static u32 RCC_PLL_N_Mask (u32 PLL_N_Factor)
{
    return (PLL_N_Factor << 6);
}

static u32 RCC_PLL_M_Mask (u32 PLL_M_Factor)
{
    return PLL_M_Factor;
} 

static u32 RCC_APB2_PreScaler_Mask (u32 PreScaler_Value)
{
    u32 loc_Temp=0;

    switch (PreScaler_Value)
    {
    case 1: loc_Temp = 0x00000000;
        break;
    case 2: loc_Temp = 0x00008000;
        break;
    case 4: loc_Temp = 0x0000A000;
        break;
    case 8: loc_Temp = 0x0000C000;
        break;
    case 16: loc_Temp = 0x000E0000;
        break;
    }

    return loc_Temp;
}

static u32 RCC_AHB_PreScaler_Mask (u32 PreScaler_Value)
{
    u32 loc_Temp=0;

    switch (PreScaler_Value)
    {
    case 1:   loc_Temp = 0x00000000;
        break;
    case 2:   loc_Temp = 0x00000080;
        break;
    case 4:   loc_Temp = 0x00000090;
        break;
    case 8:   loc_Temp = 0x000000A0;
        break;
    case 16:  loc_Temp = 0x000000B0;
        break;
    case 64:  loc_Temp = 0x000000C0;
        break;
    case 128: loc_Temp = 0x000000D0;
        break;
    case 256: loc_Temp = 0x000000E0;
        break;
    case 512: loc_Temp = 0x000000F0;
        break;
    }

    return loc_Temp;
}
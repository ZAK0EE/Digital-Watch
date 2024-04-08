/*
 ============================================================================
 Name        : RCC.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the RCC Driver
 Date        : 10/2/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "MCAL/RCC/RCC.h"
/*******************************************************************************
 *                             Definitions                                      *
 *******************************************************************************/
#define RCC_Base_ADDRESS 0x40023800

#define SysClk_HSI_READY_MASK 0x00000000
#define SysClk_HSE_READY_MASK 0x00000004
#define SysClk_PLL_READY_MASK 0x00000008

#define FIRST_BIT_CHECK_AND_SHIFT 29

#define SysClk_CLR_MASK 0xFFFFFFFC
#define SysClk_READ_MASK 0x0000000C
#define PLL_SOURCE_CLR_MASK 0xFFBFFFFF
#define PLL_CONFIG_CLR_MASK 0XF0FC8000
#define APBH_PRE_CLR_MASK 0XFFFF1FFF
#define APBL_PRE_CLR_MASK 0XFFFFE3FF
#define AHB_PRE_CLR_MASK 0XFFFFFF0F

#define AHB1_BUS_MASK 0x00000000
#define AHB2_BUS_MASK 0x20000000
#define APB1_BUS_MASK 0x40000000
#define APB2_BUS_MASK 0x60000000

#define AHB1_BUS_CHECK_MASK 0x00000000
#define AHB2_BUS_CHECK_MASK 0x00000001
#define APB1_BUS_CHECK_MASK 0x00000002
#define APB2_BUS_CHECK_MASK 0x00000003

#define PLL_Q_SHIFTING 24
#define PLL_P_SHIFTING 16
#define PLL_N_SHIFTING 6

#define PLLQ_BOUNDARY1 2
#define PLLQ_BOUNDARY2 15
#define PLLP_BOUNDARY1 2
#define PLLP_BOUNDARY2 8
#define PLLP_WRONGWAUE1 3
#define PLLP_WRONGWAUE2 5
#define PLLP_WRONGWAUE3 7
#define PLLN_BOUNDARY1 192
#define PLLN_BOUNDARY2 432
#define PLLM_BOUNDARY1 2
#define PLLM_BOUNDARY2 63

#define PPRE2_APBH_SHIFTING 13
#define PPRE1_APBL_SHIFTING 10
#define HPRE_AHB_SHIFTING 4

#define PPRE2_APBH_CLR_MASK 0xFFFF1FFF
#define PPRE1_APBL_CLR_MASK 0xFFFFE3FF
#define HPRE_AHB_CLR_MASK 0xFFFFFF0F
/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef struct
{
    uint32_t RCC_CR;
    uint32_t RCC_PLLCFGR;
    uint32_t RCC_CFGR;
    uint32_t RCC_CIR;
    uint32_t RCC_AHB1RSTR;
    uint32_t RCC_AHB2RSTR;
    uint32_t Reserved;
    uint32_t Reserved2;
    uint32_t RCC_APB1RSTR;
    uint32_t RCC_APB2RSTR;
    uint32_t Reserved3;
    uint32_t Reserved4;
    uint32_t RCC_AHB1ENR;
    uint32_t RCC_AHB2ENR;
    uint32_t Reserved5;
    uint32_t Reserved6;
    uint32_t RCC_APB1ENR;
    uint32_t RCC_APB2ENR;
    uint32_t Reserved7;
    uint32_t Reserved8;
    uint32_t RCC_AHB1LPENR;
    uint32_t RCC_AHB2LPENR;
    uint32_t Reserved9;
    uint32_t Reserved10;
    uint32_t RCC_APB1LPENR;
    uint32_t RCC_APB2LPENR;
    uint32_t Reserved11;
    uint32_t Reserved12;
    uint32_t RCC_BDCR;
    uint32_t RCC_CSR;
    uint32_t Reserved13;
    uint32_t Reserved14;
    uint32_t RCC_SSCGR;
    uint32_t RCC_PLLI2SCFGR;
} RCC_PERI_t;

/*******************************************************************************
 *                              Variables		                                *
 *******************************************************************************/
volatile RCC_PERI_t *const RCC = (volatile RCC_PERI_t *)RCC_Base_ADDRESS;

/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*
 * @brief    : Set Clock ON.
 * @param[in]: Copy_Clock The clock source to be set on. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source.
               It enables the clock according to the provided clock source.
 */
Error_enumStatus_t RCC_SET_Clock_ON(uint32_t Copy_Clock)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint32_t Loc_u32Temp = RCC->RCC_CR;

    /* Check if the provided clock source is valid */
    if (Copy_Clock != CLOCK_HSI && Copy_Clock != CLOCK_HSE && Copy_Clock != CLOCK_PLL)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Clear the previous clock configuration and set the new clock source */
        Loc_u32Temp &= ~Copy_Clock;
        Loc_u32Temp |= Copy_Clock;
        RCC->RCC_CR = Loc_u32Temp;
    }

    /* Return the status of the clock operation */
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Set Clock OFF.
 * @param[in]: Copy_Clock The clock source to be turned off. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of turning the clock off.
 * @details  : This function turns off the specified clock source.
               It disables the clock according to the provided clock source.
 */
Error_enumStatus_t RCC_SET_Clock_OFF(uint32_t Copy_Clock)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint32_t Loc_u32Temp = RCC->RCC_CR;

    /* Check if the provided clock source is valid */
    if (Copy_Clock != CLOCK_HSI && Copy_Clock != CLOCK_HSE && Copy_Clock != CLOCK_PLL)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Clear the clock source to turn it off */
        Loc_u32Temp &= ~Copy_Clock;
        RCC->RCC_CR = Loc_u32Temp;
    }

    /* Return the status of the clock operation */
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Read if the Clock is Ready or not
 * @param[in]:  Copy_Clock The clock source to be checked. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of selecting the system clock.
 */
Error_enumStatus_t RCC_READ_ClockReadyState(uint32_t Copy_ReadyClock)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Check if the provided ready clock source is valid */
    if (Copy_ReadyClock != READY_HSI && Copy_ReadyClock != READY_HSE && Copy_ReadyClock != READY_PLL)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Initialize a timer for timeout */
        uint32_t timer = 1000; /* Set a timeout value for the clock readiness check. */

        /* Wait for the clock to become ready with a timeout */
        while (timer && !(RCC->RCC_CR & Copy_ReadyClock))
        {
            timer--; /* Decrement the timer. */
        }

        /* Check if the clock is still not ready after the timeout */
        if (!(RCC->RCC_CR & Copy_ReadyClock))
        {
            Loc_enumReturnStatus = Status_enumNotOk;
        }
        else
        {
            /* Clock is ready, do nothing */
        }
    }

    /* Return the status of the clock readiness */
    return Loc_enumReturnStatus; /* Return the error status indicating the readiness of the clock. */
}

/*
 * @brief    : Select System Clock.
 * @param[in]: Copy_SysClk The system clock source to be selected. It can be SysClk_HSI_MASK, SysClk_HSE_MASK, or SysClk_PLL_MASK.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of selecting the system clock.
 * @details  : This function selects the system clock source among the available options: HSI, HSE, or PLL.
 */
Error_enumStatus_t RCC_Select_Sysclk(uint32_t Copy_SysClk)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Register to hold system clock configuration */
    uint32_t Loc_u32Temp = RCC->RCC_CFGR;

    /* Check if the provided system clock source is valid */
    if (Copy_SysClk != SysClk_HSI_MASK && Copy_SysClk != SysClk_HSE_MASK && Copy_SysClk != SysClk_PLL_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Configure the system clock source */
        Loc_u32Temp &= SysClk_CLR_MASK; /* Clear the bits related to system clock configuration. */
        Loc_u32Temp |= Copy_SysClk;     /* Set the bits for the selected system clock source. */
        RCC->RCC_CFGR = Loc_u32Temp;    /* Update the RCC_CFGR register with the new system clock configuration. */
    }

    /* Return the status of the system clock source selection */
    return Loc_enumReturnStatus; /* Return the error status indicating the success or failure of system clock source selection. */
}

/*
 * @brief    : Configure PLL Source.
 * @param[in]: Copy_PLLSrc The PLL source to be configured. It can be PLL_SRC_HSI or PLL_SRC_HSE.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of configuring PLL source.
 * @details  : This function configures the PLL source to be either HSI or HSE.
 */
Error_enumStatus_t RCC_Config_PLLSrc(uint32_t Copy_PLLSrc)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Register to hold PLL configuration */
    uint32_t Loc_u32Temp = RCC->RCC_PLLCFGR;

    /* Check if the provided PLL source is valid */
    if (Copy_PLLSrc != PLL_SRC_HSI && Copy_PLLSrc != PLL_SRC_HSE)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        Loc_u32Temp &= PLL_SOURCE_CLR_MASK;
        Loc_u32Temp |= Copy_PLLSrc;
        RCC->RCC_PLLCFGR = Loc_u32Temp;
    }

    /* Return the status of the PLL clock source configuration. */
    return Loc_enumReturnStatus; /* Return the error status indicating the success or failure of PLL clock source configuration. */
}

/*
 * @brief    : Configure PLL Parameters.
 * @param[in]: PLLM, PLLN, PLLP, PLLQ The parameters to configure PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of configuring PLL parameters.
 * @details  : This function configures the parameters (M, N, P, Q) for the PLL.
 * @note     : Ranges of Paramters 2<=PLLM<=63 ,192<=PLLN<=432 , 2<=PLLQ<=15 , PLLP = 2,4,6,8
 */
Error_enumStatus_t RCC_Config_PLLParamters(uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Initialize a variable to store the temporary value for PLL configuration. */
    uint32_t Loc_u32Temp = RCC->RCC_PLLCFGR;
    /* Validate the PLL parameters */
    if (PLLQ < PLLQ_BOUNDARY1 || PLLQ > PLLQ_BOUNDARY2)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else if (PLLP < PLLP_BOUNDARY1 || PLLP > PLLP_BOUNDARY2 || PLLP == PLLP_WRONGWAUE1 || PLLP == PLLP_WRONGWAUE2 || PLLP == PLLP_WRONGWAUE3)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else if (PLLN < PLLN_BOUNDARY1 || PLLN > PLLN_BOUNDARY2)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else if (PLLM < PLLM_BOUNDARY1 || PLLM > PLLM_BOUNDARY2)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Convert PLLP value */
        PLLP = (uint32_t)(PLLP / 2) - 1;
        Loc_u32Temp &= PLL_CONFIG_CLR_MASK;
        /* Calculate PLL configuration value */
        Loc_u32Temp = PLLQ << PLL_Q_SHIFTING;
        Loc_u32Temp |= PLLP << PLL_P_SHIFTING;
        Loc_u32Temp |= PLLN << PLL_N_SHIFTING;
        Loc_u32Temp |= PLLM;
        /* Configure the PLL with the calculated value */
        RCC->RCC_PLLCFGR = Loc_u32Temp;
    }
    /* Return the status of the PLL parameter configuration. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Configure APB high-speed prescaler BUS Prescaler APB2.
 * @param[in]: CopyPreScalerValue  Prescaler from RCC_APB_HS_PRE_t type.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting prescaler value.
 */
Error_enumStatus_t RCC_Config_APBH_BusPrescaler(uint32_t CopyPreScalerValue)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Register to hold the current RCC configuration */
    uint32_t Loc_u32Temp = RCC->RCC_CFGR;

    /* Validate the provided prescaler value */
    if ((CopyPreScalerValue > APB_H_16)|| ((CopyPreScalerValue>APB_H_1)&& (CopyPreScalerValue<APB_H_2)))
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Configure the APB high-speed bus prescaler */
        Loc_u32Temp &= APBH_PRE_CLR_MASK;
        Loc_u32Temp |= CopyPreScalerValue;
        RCC->RCC_CFGR = Loc_u32Temp;
    }

    /* Return the status of the APB high-speed bus prescaler configuration. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Configure APB Low speed prescaler BUS Prescaler APB1.
 * @param[in]: CopyPreScalerValue  Prescaler from RCC_APB_LW_PRE_t type.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting prescaler value.
 */
Error_enumStatus_t RCC_Config_APBL_BusPrescaler(uint32_t CopyPreScalerValue)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Register to hold the current RCC configuration */
    uint32_t Loc_u32Temp = RCC->RCC_CFGR;

    /* Validate the provided prescaler value */
    if ((CopyPreScalerValue > APB_L_16)|| ((CopyPreScalerValue>APB_L_1)&& (CopyPreScalerValue<APB_L_2)))
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Configure the APB low-speed bus prescaler */
        Loc_u32Temp &= APBL_PRE_CLR_MASK;
        Loc_u32Temp |= CopyPreScalerValue;
        RCC->RCC_CFGR = Loc_u32Temp;
    }

    /* Return the status of the APB low-speed bus prescaler configuration. */
    return Loc_enumReturnStatus;
}
/*
 * @brief    : Configure AHB prescalerr BUS Prescaler AHB.
 * @param[in]: CopyPreScalerValue  Prescaler from RCC_AHB_PRE_t type.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting prescaler value.
 */
Error_enumStatus_t RCC_Config_AHB_BusPrescaler(uint32_t CopyPreScalerValue)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;

    /* Register to hold the current RCC configuration */
    uint32_t Loc_u32Temp = RCC->RCC_CFGR;

    /* Validate the provided prescaler value */
    if ((CopyPreScalerValue > AHB_512)|| ((CopyPreScalerValue>AHB_1)&& (CopyPreScalerValue<AHB_2)))
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Configure the AHB bus prescaler */
        Loc_u32Temp &= AHB_PRE_CLR_MASK;
        Loc_u32Temp |= CopyPreScalerValue;
        RCC->RCC_CFGR = Loc_u32Temp;
    }

    /* Return the status of the AHB bus prescaler configuration. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Enable AHB1 Peripheral.
 * @param[in]: Copy_AHB1PeripheralName The AHB1 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the AHB1 peripheral.
 * @details  : This function enables the specified peripheral connected to the AHB1 bus.
 */
Error_enumStatus_t RCC_Enable_AHB1_Peripheral(uint32_t Copy_AHB1PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Read the current value of RCC_AHB1ENR register. */
    uint32_t Copy_AHB1Peripheral = RCC->RCC_AHB1ENR;

    /* Validate the provided prescaler value */
    if ((Copy_AHB1PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != AHB1_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
        /* Clear the reserved bits corresponding to testing */      
        Copy_AHB1PeripheralName &= ~(AHB1_BUS_MASK);

        /* Clear the bits corresponding to the AHB1 peripheral to be enabled. */
        Copy_AHB1Peripheral &= ~(Copy_AHB1PeripheralName);

        /* Set the bits corresponding to the AHB1 peripheral to be enabled. */
        Copy_AHB1Peripheral |= Copy_AHB1PeripheralName;

        /* Write the updated value back to RCC_AHB1ENR register to enable the AHB1 peripheral. */
        RCC->RCC_AHB1ENR = Copy_AHB1Peripheral;

        /* Return the status indicating the success of enabling the AHB1 peripheral. */
        
    }
    return Loc_enumReturnStatus;
}

/*
 * @brief    : Enable AHB2 Peripheral.
 * @param[in]: Copy_AHB2PeripheralName The AHB2 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the AHB2 peripheral.
 * @details  : This function enables the specified peripheral connected to the AHB2 bus.
 */
Error_enumStatus_t RCC_Enable_AHB2_Peripheral(uint32_t Copy_AHB2PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Read the current value of RCC_AHB2ENR register. */
    uint32_t Copy_AHB2Peripheral = RCC->RCC_AHB2ENR;
    /* Validate the provided prescaler value */
    if ((Copy_AHB2PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != AHB2_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */              
    Copy_AHB2PeripheralName &= ~(AHB2_BUS_MASK);

    /* Clear the bits corresponding to the AHB2 peripheral to be enabled. */
    Copy_AHB2Peripheral &= ~(Copy_AHB2PeripheralName);

    /* Set the bits corresponding to the AHB2 peripheral to be enabled. */
    Copy_AHB2Peripheral |= Copy_AHB2PeripheralName;

    /* Write the updated value back to RCC_AHB2ENR register to enable the AHB2 peripheral. */
    RCC->RCC_AHB2ENR = Copy_AHB2Peripheral;

    /* Return the status indicating the success of enabling the AHB2 peripheral. */
    
    }
    return Loc_enumReturnStatus;
}

/*
 * @brief    Enable APB1 Peripheral.
 * @param[in]: Copy_APB1PeripheralName The APB1 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the APB1 peripheral.
 * @details  : This function enables the specified peripheral connected to the APB1 bus.
 */
Error_enumStatus_t RCC_Enable_APB1_Peripheral(uint32_t Copy_APB1PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Read the current value of RCC_APB1ENR register. */
    uint32_t Copy_APB1Peripheral = RCC->RCC_APB1ENR;
    /* Validate the provided prescaler value */
    if ((Copy_APB1PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != APB1_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */             
    Copy_APB1PeripheralName &= ~(APB1_BUS_MASK);

    /* Clear the bits corresponding to the APB1 peripheral to be enabled. */
    Copy_APB1Peripheral &= ~(Copy_APB1PeripheralName);

    /* Set the bits corresponding to the APB1 peripheral to be enabled. */
    Copy_APB1Peripheral |= Copy_APB1PeripheralName;

    /* Write the updated value back to RCC_APB1ENR register to enable the APB1 peripheral. */
    RCC->RCC_APB1ENR = Copy_APB1Peripheral;
    }
    /* Return the status indicating the success of enabling the APB1 peripheral. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    Enable APB2 Peripheral.
 * @param[in]: Copy_APB2PeripheralName The APB2 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the APB2 peripheral.
 * @details  : This function enables the specified peripheral connected to the APB2 bus.
 */
Error_enumStatus_t RCC_Enable_APB2_Peripheral(uint32_t Copy_APB2PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
   /* Read the current value of RCC_APB2ENR register. */
    uint32_t Copy_APB2Peripheral = RCC->RCC_APB2ENR;

    /* Validate the provided prescaler value */
    if ((Copy_APB2PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != APB2_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */             
    Copy_APB2PeripheralName &= ~(APB2_BUS_MASK);

    /* Clear the bits corresponding to the APB2 peripheral to be enabled. */
    Copy_APB2Peripheral &= ~(Copy_APB2PeripheralName);

    /* Set the bits corresponding to the APB2 peripheral to be enabled. */
    Copy_APB2Peripheral |= Copy_APB2PeripheralName;

    /* Write the updated value back to RCC_APB2ENR register to enable the APB2 peripheral. */
    RCC->RCC_APB2ENR = Copy_APB2Peripheral;
    }
    /* Return the status indicating the success of enabling the APB2 peripheral. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    Disable AHB1 Peripheral.
 * @param[in]: Copy_AHB1PeripheralName The AHB1 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the AHB1 peripheral.
 * @details  : This function disables the specified peripheral connected to the AHB1 bus.
 */
Error_enumStatus_t RCC_Disable_AHB1_Peripheral(uint32_t Copy_AHB1PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Read the current value of RCC_AHB1ENR register. */
    uint32_t Copy_AHB1Peripheral = RCC->RCC_AHB1ENR;

    /* Validate the provided prescaler value */
    if ((Copy_AHB1PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != AHB1_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */      
    Copy_AHB1PeripheralName &= ~(AHB1_BUS_MASK);

    /* Clear the bits corresponding to the AHB1 peripheral to be disabled. */
    Copy_AHB1Peripheral &= ~(Copy_AHB1PeripheralName);

    /* Write the updated value back to RCC_AHB1ENR register to disable the AHB1 peripheral. */
    RCC->RCC_AHB1ENR = Copy_AHB1Peripheral;
    }
    /* Return the status indicating the success of disabling the AHB1 peripheral. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    Disable AHB2 Peripheral.
 * @param[in]: Copy_AHB2PeripheralName The AHB2 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the AHB2 peripheral.
 * @details  : This function disables the specified peripheral connected to the AHB2 bus.
 */
Error_enumStatus_t RCC_Disable_AHB2_Peripheral(uint32_t Copy_AHB2PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Read the current value of RCC_AHB2ENR register. */
    uint32_t Copy_AHB2Peripheral = RCC->RCC_AHB2ENR;
    /* Validate the provided prescaler value */
    if ((Copy_AHB2PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != AHB2_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */              
    Copy_AHB2PeripheralName &= ~(AHB2_BUS_MASK);

    /* Clear the bits corresponding to the AHB2 peripheral to be disabled. */
    Copy_AHB2Peripheral &= ~(Copy_AHB2PeripheralName);

    /* Write the updated value back to RCC_AHB2ENR register to disable the AHB2 peripheral. */
    RCC->RCC_AHB2ENR = Copy_AHB2Peripheral;
    }
    /* Return the status indicating the success of disabling the AHB2 peripheral. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    Disable APB1 Peripheral.
 * @param[in]: Copy_APB1PeripheralName The APB1 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the APB1 peripheral.
 * @details  : This function disables the specified peripheral connected to the APB1 bus.
 */
Error_enumStatus_t RCC_Disable_APB1_Peripheral(uint32_t Copy_APB1PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    /* Read the current value of RCC_APB1ENR register. */
    uint32_t Copy_APB1Peripheral = RCC->RCC_APB1ENR;
    /* Validate the provided prescaler value */
    if ((Copy_APB1PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != APB1_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */             
    Copy_APB1PeripheralName &= ~(APB1_BUS_MASK);

    /* Clear the bits corresponding to the APB1 peripheral to be disabled. */
    Copy_APB1Peripheral &= ~(Copy_APB1PeripheralName);

    /* Write the updated value back to RCC_APB1ENR register to disable the APB1 peripheral. */
    RCC->RCC_APB1ENR = Copy_APB1Peripheral;
    }
    /* Return the status indicating the success of disabling the APB1 peripheral. */
    return Loc_enumReturnStatus;
}

/*
 * @brief    Disable APB2 Peripheral.
 * @param[in]: Copy_APB2PeripheralName The APB2 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the APB2 peripheral.
 * @details  : This function disables the specified peripheral connected to the APB2 bus.
 */
Error_enumStatus_t RCC_Disable_APB2_Peripheral(uint32_t Copy_APB2PeripheralName)
{
    /* Initialize a variable to store the error status. */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
   /* Read the current value of RCC_APB2ENR register. */
    uint32_t Copy_APB2Peripheral = RCC->RCC_APB2ENR;
    /* Validate the provided prescaler value */
    if ((Copy_APB2PeripheralName >> FIRST_BIT_CHECK_AND_SHIFT) != APB2_BUS_CHECK_MASK)
    {
        Loc_enumReturnStatus = Status_enumWrongInput;
    }
    else
    {
    /* Clear the reserved bits corresponding to testing */             
    Copy_APB2PeripheralName &= ~(APB2_BUS_MASK);

    /* Clear the bits corresponding to the APB2 peripheral to be disabled. */
    Copy_APB2Peripheral &= ~(Copy_APB2PeripheralName);
    /* Write the updated value back to RCC_APB2ENR register to disable the APB2 peripheral. */
    RCC->RCC_APB2ENR = Copy_APB2Peripheral;
    }
    /* Return the status indicating the success of disabling the APB2 peripheral. */
    return Loc_enumReturnStatus;
}

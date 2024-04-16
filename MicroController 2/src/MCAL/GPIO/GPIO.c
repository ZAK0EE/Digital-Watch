#include "MCAL/GPIO/GPIO.h"

/*Ports Base Adresses*/
#define GPIO_PORT_A_BASE_ADDRESS 0x40020000
#define GPIO_PORT_B_BASE_ADDRESS 0x40020400
#define GPIO_PORT_C_BASE_ADDRESS 0x40020800

/*MASKS*/
#define GPIO_MODER_MASK   0x00000003
#define GPIO_OTYPR_MASK   0x00000001
#define GPIO_OSPEEDR_MASK 0x00000003
#define GPIO_PUPDR_MASK   0x00000003

#define GPIO_MODER_SHIFT_MASK   0x00000002
#define GPIO_OTYPR_SHIFT_MASK   0x00000001
#define GPIO_OSPEEDR_SHIFT_MASK 0x00000002
#define GPIO_PUPDR_SHIFT_MASK   0x00000002

#define GPIO_BSRR_MASK 0x00000010

#define GPIO_4_BIT_MASK    0x0000000FUL

typedef struct
{

    u32 MODER_MASK;
    u32 OTYPR_MASK;
    u32 PUPDR_MASK;

}Masks_t;

/*Peripheral Registers*/
typedef struct
{
    
    u32 GPIO_MODER;
    u32 GPIO_OTYPER;
    u32 GPIO_OSPEEDR;
    u32 GPIO_PUPDR;
    u32 GPIO_IDR;
    u32 GPIO_ODR;
    u32 GPIO_BSRR;
    u32 GPIO_LCKR;
    u32 AFRL;
    u32 AFRH;

}GPIO_Registers_t;

static volatile GPIO_Registers_t * const GPIO [3] = {(volatile GPIO_Registers_t *const)GPIO_PORT_A_BASE_ADDRESS,(volatile GPIO_Registers_t *const)GPIO_PORT_B_BASE_ADDRESS,(volatile GPIO_Registers_t *const)GPIO_PORT_C_BASE_ADDRESS};

/*STATIC FUNCTIONS*/
static Masks_t Generate_Mask (u32 config, u32 pin);
static Masks_t Generate_CLEAR_Mask (u32 config, u32 pin);

Error_t GPIO_Init_Pin (GPIO_Pin_Confg_t * Pin_Config)
{
    Error_t Error = ERROR_Ok;
    
    Masks_t Clear_Mask, Mask;

    Clear_Mask = Generate_CLEAR_Mask(Pin_Config->mode,Pin_Config->pin);
    Mask = Generate_Mask(Pin_Config->mode,Pin_Config->pin);
    
    u32 Temp_Reg = GPIO[Pin_Config->port]->GPIO_MODER;
    Temp_Reg &= Clear_Mask.MODER_MASK;
    Temp_Reg |= Mask.MODER_MASK;
    GPIO[Pin_Config->port]->GPIO_MODER = Temp_Reg;

    Temp_Reg = GPIO[Pin_Config->port]->GPIO_OTYPER;
    Temp_Reg &= Clear_Mask.OTYPR_MASK;
    Temp_Reg |= Mask.OTYPR_MASK;
    GPIO[Pin_Config->port]->GPIO_OTYPER = Temp_Reg;

    Temp_Reg = GPIO[Pin_Config->port]->GPIO_PUPDR;
    Temp_Reg &= Clear_Mask.PUPDR_MASK;
    Temp_Reg |= Mask.PUPDR_MASK;
    GPIO[Pin_Config->port]->GPIO_PUPDR = Temp_Reg;

    Temp_Reg = GPIO[Pin_Config->port]->GPIO_OSPEEDR;
    Temp_Reg &= (~(0x00000003)<<(Pin_Config->pin*2));
    Temp_Reg |= (Pin_Config->speed << (Pin_Config->pin*2));
    GPIO[Pin_Config->port]->GPIO_OSPEEDR = Temp_Reg; 

    /*AF*/
    u32 AF_Pin = Pin_Config->pin % 8;
    if (Pin_Config->pin > 7)
    {
        Temp_Reg  = GPIO[Pin_Config->port]->AFRH;
        Temp_Reg &= (~((GPIO_4_BIT_MASK) << (AF_Pin * 4)));
        Temp_Reg |= (Pin_Config->AF << (AF_Pin*4));
        GPIO[Pin_Config->port]->AFRH = Temp_Reg;
    }
    else
    {
        Temp_Reg  = GPIO[Pin_Config->port]->AFRL;
        Temp_Reg &= (~(GPIO_4_BIT_MASK) << (Pin_Config->pin * 4));
        Temp_Reg |= (Pin_Config->AF << (AF_Pin*4));
        GPIO[Pin_Config->port]->AFRL = Temp_Reg;
    }
    
    
    

    return Error;
}

Error_t GPIO_Set_Pin_State(u32 GPIO_PORT, u32 GPIO_PIN, u32 GPIO_State)
{
    /*Add Validation*/
    Error_t Error_State = ERROR_Ok;
    switch (GPIO_State)
    {
    case GPIO_PIN_ON:
        GPIO[GPIO_PORT]->GPIO_BSRR = (1 << GPIO_PIN);
        break;
    case GPIO_PIN_OFF:
        GPIO[GPIO_PORT]->GPIO_BSRR = (1 << (GPIO_PIN + GPIO_BSRR_MASK));
        break;
    
    default:
        break;
    }
    return Error_State;
}

Error_t GPIO_Get_Pin_State(u8 GPIO_PORT, u8 GPIO_PIN, u8 *Pin_State)
{
    /*Validation*/
    Error_t Error_State = ERROR_Ok;
    *Pin_State = ((GPIO[GPIO_PORT]->GPIO_IDR) >> GPIO_PIN ) & 1UL;
    return Error_State;
}

static Masks_t Generate_Mask (u32 config, u32 pin)
{
    Masks_t Mask;

    switch (config)
    {
    case GPIO_CONFG_INPUT_FLOATING: Mask.MODER_MASK = 0x00000000;
                                    Mask.OTYPR_MASK = 0x00000000;
                                    Mask.PUPDR_MASK = 0x00000000;
        break;
    case GPIO_CONFG_INPUT_PU: Mask.MODER_MASK = 0x00000000;
                              Mask.OTYPR_MASK = 0x00000000;
                              Mask.PUPDR_MASK = (0x00000001 << (pin * 2));
        break;
    case GPIO_CONFG_INPUT_PD: Mask.MODER_MASK = 0x00000000;
                              Mask.OTYPR_MASK = 0x00000000;
                              Mask.PUPDR_MASK = (0x00000002 << (pin * 2));
        break;
    case GPIO_CONFG_ANALOG: Mask.MODER_MASK = (0x00000003 << (pin * 2));
                            Mask.OTYPR_MASK = 0x00000000;
                            Mask.PUPDR_MASK = 0x00000000;
        break;
    case GPIO_CONFG_OUTPUT_OD: Mask.MODER_MASK = (0x00000001 << (pin * 2));
                               Mask.OTYPR_MASK = (0x00000001 << pin);
                               Mask.PUPDR_MASK = 0x00000000;
        break;
    case GPIO_CONFG_OUTPUT_OD_PU: Mask.MODER_MASK = (0x00000001 << (pin * 2));
                                  Mask.OTYPR_MASK = (0x00000001 << pin);
                                  Mask.PUPDR_MASK = (0x00000001 << (pin * 2));
        break;
    case GPIO_CONFG_OUTPUT_OD_PD: Mask.MODER_MASK = (0x00000001 << (pin * 2));
                                  Mask.OTYPR_MASK = (0x00000001 << pin);
                                  Mask.PUPDR_MASK = (0x00000002 << (pin * 2));
        break;
    case GPIO_CONFG_OUTPUT_PP: Mask.MODER_MASK = (0x00000001 << (pin * 2));
                               Mask.OTYPR_MASK = 0x00000000;
                               Mask.PUPDR_MASK = 0x00000000;
        break;
    case GPIO_CONFG_OUTPUT_PP_PU: Mask.MODER_MASK = (0x00000001 << (pin * 2));
                                  Mask.OTYPR_MASK = 0x00000000;
                                  Mask.PUPDR_MASK = (0x00000001 << (pin * 2));
        break;
    case GPIO_CONFG_OUTPUT_PP_PD: Mask.MODER_MASK = (0x00000001 << (pin * 2));
                                  Mask.OTYPR_MASK = 0x00000000;
                                  Mask.PUPDR_MASK = (0x00000002 << (pin * 2));
        break;
    case GPIO_CONFG_AF_OD: Mask.MODER_MASK = (0x00000002 << (pin * 2));
                           Mask.OTYPR_MASK = (0x00000001 << pin);
                           Mask.PUPDR_MASK = 0x00000000;
        break;
    case GPIO_CONFG_AF_OD_PU: Mask.MODER_MASK = (0x00000002 << (pin * 2));
                              Mask.OTYPR_MASK = (0x00000001 << pin);
                              Mask.PUPDR_MASK = (0x00000001 << (pin * 2));
        break;
    case GPIO_CONFG_AF_OD_PD: Mask.MODER_MASK = (0x00000002 << (pin * 2));
                              Mask.OTYPR_MASK = (0x00000001 << pin);
                              Mask.PUPDR_MASK = (0x00000002 << (pin * 2));
        break;
    case GPIO_CONFG_AF_PP: Mask.MODER_MASK = (0x00000002 << (pin * 2));
                           Mask.OTYPR_MASK = 0x00000000;
                           Mask.PUPDR_MASK = 0x00000000;
        break;
    case GPIO_CONFG_AF_PP_PU: Mask.MODER_MASK = (0x00000002 << (pin * 2));
                              Mask.OTYPR_MASK = 0x00000000;
                              Mask.PUPDR_MASK = (0x00000001 << (pin * 2));
        break;
    case GPIO_CONFG_AF_PP_PD: Mask.MODER_MASK = (0x00000002 << (pin * 2));
                              Mask.OTYPR_MASK = 0x00000000;
                              Mask.PUPDR_MASK = (0x00000002 << (pin * 2));
        break;
    }

    return Mask;
}

static Masks_t Generate_CLEAR_Mask (u32 config, u32 pin)
{
    Masks_t Mask;

    switch (config)
    {
    case GPIO_CONFG_INPUT_FLOATING: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                                    Mask.OTYPR_MASK = 0xFFFFFFFF;
                                    Mask.PUPDR_MASK = 0xFFFFFFFF;
        break;
    case GPIO_CONFG_INPUT_PU: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                              Mask.OTYPR_MASK = 0xFFFFFFFF;
                              Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_INPUT_PD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                              Mask.OTYPR_MASK = 0xFFFFFFFF;
                              Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_ANALOG: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                            Mask.OTYPR_MASK = 0xFFFFFFFF;
                            Mask.PUPDR_MASK = 0xFFFFFFFF;
        break;
    case GPIO_CONFG_OUTPUT_OD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                               Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                               Mask.PUPDR_MASK = 0xFFFFFFFF;
        break;
    case GPIO_CONFG_OUTPUT_OD_PU: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                                  Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                                  Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_OUTPUT_OD_PD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                                  Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                                  Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_OUTPUT_PP: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                               Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                               Mask.PUPDR_MASK = 0xFFFFFFFF;
        break;
    case GPIO_CONFG_OUTPUT_PP_PU: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                                  Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                                  Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_OUTPUT_PP_PD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                                  Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                                  Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_AF_OD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                           Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                           Mask.PUPDR_MASK = 0xFFFFFFFF;
        break;
    case GPIO_CONFG_AF_OD_PU: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                              Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                              Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_AF_OD_PD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                              Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                              Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_AF_PP: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                           Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                           Mask.PUPDR_MASK = 0xFFFFFFFF;
        break;
    case GPIO_CONFG_AF_PP_PU: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                              Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                              Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    case GPIO_CONFG_AF_PP_PD: Mask.MODER_MASK = (~(0x00000003 << (pin*2) ));
                              Mask.OTYPR_MASK = (~(0x00000001 << (pin) ));
                              Mask.PUPDR_MASK = (~(0x00000003 << (pin*2) ));
        break;
    }

    return Mask;
}
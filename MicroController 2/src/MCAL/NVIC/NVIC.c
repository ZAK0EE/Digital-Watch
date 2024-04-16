/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "NVIC.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define NVIC_BASE_ADDRESS        (u32)0xE000E100
#define SCB_BASE_ADDRESS         (u32)0xE000E008


#define LOWER_3_BITS_MASK                (u32)0x07
#define LOWER_5_BITS_MASK                (u32)0X1F

#define _NVIC_PRIO_BITS                  (u32)4
#define _SCB_AIRCR_PRIGROUP_MASK         (u32)0x700
#define _SCB_AIRCR_PRIGROUP_POS          (u32)8
#define _SCB_AIRCR_VECTKEY_MASK          ((u32)0x7E800000)
#define _SCB_AIRCR_SYSTEM_RESET_REQ_MASK ((u32)0x4)
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct {
    volatile u32 ISER[8];      // Interrupt Set Enable Register
    u32 RESERVED0[24];         // Reserved space to align with other NVIC registers
    volatile u32 ICER[8];      // Interrupt Clear Enable Register
    u32 RESERVED1[24];         // Reserved space
    volatile u32 ISPR[8];      // Interrupt Set Pending Register
    u32 RESERVED2[24];         // Reserved space
    volatile u32 ICPR[8];      // Interrupt Clear Pending Register
    u32 RESERVED3[24];         // Reserved space
    volatile u32 IABR[8];      // Interrupt Active Bit Register
    u32 RESERVED4[56];         // Reserved space
    volatile u8 IP[240];       // Interrupt Priority Registers (8-bit wide)
    u32 RESERVED5[644];        // Reserved space
    volatile u32 STIR;         // Software Trigger Interrupt Register
} NVIC_regs_t;


typedef struct {
    volatile u32 CPUID;          // CPU ID Base Register
    volatile u32 ICSR;           // Interrupt Control and State Register
    volatile u32 VTOR;           // Vector Table Offset Register
    volatile u32 AIRCR;          // Application Interrupt and Reset Control Register
    volatile u32 SCR;            // System Control Register
    volatile u32 CCR;            // Configuration and Control Register
    volatile u8 SHP[12];     // System Handlers Priority Registers (4x 8-bit wide)
    volatile u32 SHCSR;          // System Handler Control and State Register
    volatile u32 CFSR;           // Configurable Fault Status Register
    volatile u32 HFSR;           // HardFault Status Register
    volatile u32 DFSR;           // Debug Fault Status Register
    volatile u32 MMFAR;          // Memory Management Fault Address Register
    volatile u32 BFAR;           // Bus Fault Address Register
    volatile u32 AFSR;           // Auxiliary Fault Status Register
} SCB_regs_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

NVIC_regs_t volatile* const NVIC = (volatile NVIC_regs_t*)NVIC_BASE_ADDRESS;
SCB_regs_t  volatile* const SCB  = (volatile SCB_regs_t* )SCB_BASE_ADDRESS;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
void __DSB(void)
{
  __asm volatile ("dsb 0xF":::"memory");
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

//Enable an external interrupt
void NVIC_EnableIRQ(IRQn_t IRQn)    
{
    NVIC->ISER[((u32)IRQn) >> 5] = (1 << (((u32)IRQn) & LOWER_5_BITS_MASK));
}           

//Disable an external interrupt
void NVIC_DisableIRQ(IRQn_t IRQn)           
{
    NVIC->ICER[((u32)IRQn) >> 5] = (1 << (((u32)IRQn) & LOWER_5_BITS_MASK));
}          

//Set IRQn pending
void NVIC_SetPendingIRQ(IRQn_t IRQn)
{
    NVIC->ISPR[((u32)IRQn) >> 5] = (1 << (((u32)IRQn) & LOWER_5_BITS_MASK));
}   

//Clear IRQn pending status
void NVIC_ClearPendingIRQ(IRQn_t IRQn)
{
    NVIC->ICPR[((u32)IRQn) >> 5] = (1 << (((u32)IRQn) & LOWER_5_BITS_MASK));
}              

//Return true (IRQ-Number) if IRQn is pending
u32 NVIC_GetPendingIRQ(IRQn_t IRQn)
{
   return ((u32) (NVIC->ISPR[((u32)IRQn) >> 5] & (1 << (((u32)IRQn) & LOWER_5_BITS_MASK))) ? 1 : 0);
}                  

//Set the priority of an interrupt
void NVIC_SetPriority(IRQn_t IRQn, u32 priority)
{
    if(IRQn < 0)
    {
        SCB->SHP[((u32)(IRQn) & 0xF) - 4] = ((priority << (8 - _NVIC_PRIO_BITS)) & 0xff);
    }
    else
    {
        NVIC->IP[(u32)IRQn] = ((priority << (8 - _NVIC_PRIO_BITS)) & 0xff);
    }
}

//Read priority of IRQn
u32 NVIC_GetPriority(IRQn_t IRQn)
{
    u32 Loc_read = 0;
    if(IRQn < 0)
    {
        Loc_read =  (u32)(SCB->SHP[((u32)(IRQn) & 0xF) - 4] >> (8 - _NVIC_PRIO_BITS));
    }
    else
    {
        Loc_read =  (u32)(NVIC->IP[(u32)IRQn] >> (8 - _NVIC_PRIO_BITS));
    }
    return Loc_read;
}                  

//Clear PRIMASK to enable interrupts
void __enable_irq(void)
{
    __asm volatile ("cpsie i");
}
//Set PRIMASK to disable all interrupts                         
void __disable_irq(void)
{
    __asm volatile ("cpsid i");
}                              

//Set priority grouping configuration
void NVIC_SetPriorityGrouping(u32 PriorityGroup)
{
    u32 PriorityGroupTmp = (PriorityGroup & LOWER_3_BITS_MASK);
    u32 tmpReg = 0;

    tmpReg = SCB->AIRCR;
    tmpReg &= ~(_SCB_AIRCR_PRIGROUP_MASK);
    tmpReg |= (PriorityGroupTmp << _SCB_AIRCR_PRIGROUP_POS);
    tmpReg |= _SCB_AIRCR_VECTKEY_MASK;
    SCB->AIRCR = tmpReg;
}  

u32 NVIC_GetPriorityGrouping(void)
{
    return ((SCB->AIRCR & _SCB_AIRCR_PRIGROUP_MASK) >> _SCB_AIRCR_PRIGROUP_POS);
}

//Return the IRQ number of the active interrupt
u32 NVIC_GetActive(IRQn_t IRQn)
{
    return ((u32) (NVIC->IABR[((u32)IRQn) >> 5] & (1 << (((u32)IRQn) & LOWER_5_BITS_MASK))) ? 1 : 0);
}

//Reset the system
void NVIC_SystemReset(void)
{
    __DSB();

    SCB->AIRCR = _SCB_AIRCR_VECTKEY_MASK | 
                (SCB->AIRCR & _SCB_AIRCR_PRIGROUP_MASK) | 
                _SCB_AIRCR_SYSTEM_RESET_REQ_MASK;

    __DSB();

    while(1);           
}                         
#include "../src/HAL/Switch/SWITCH.h"
#include "../lib/StdTypes.h"
#include "../src/HAL/LED/LED.h"

void CommPublish(u8 Switch){}

#define INC_SWITCH  0x10
#define MODE_SWITCH 0x20
#define EDIT_SWITCH 0x30

#define SAMPLING_NUM 5

typedef enum
{
    Inc,
    Mode,
    Edit,
    _Num_Of_Switches,
}Switches_t;

u8 Current_State = 0;
static u8 Prev_State    [_Num_Of_Switches] = {0};
static u8 State         [_Num_Of_Switches] = {0};  
u8 Counter              [_Num_Of_Switches] = {0};

void HSwitch(void)
{
    
    for (u8 iter = 0; iter < _Num_Of_Switches; ++iter)
    {
        SWITCH_Get_State(iter , &Current_State);
        
        if(Current_State == Prev_State[iter])
        {
            ++Counter[iter];
        }
        else
        {
            Counter[iter] = 0;
        }
        

        if (Counter[iter] % SAMPLING_NUM == 0)
        {
            State[iter] = Current_State;
        }

        Prev_State[iter] = Current_State;

    }
    
}

void Read_Switch(void)
{
    if (State[Inc] == Switch_Pressed)
    {
        CommPublish(INC_SWITCH);
    }
    
    if (State[Mode] == Switch_Pressed)
    {
        CommPublish(MODE_SWITCH);
    }
    
    if (State[Edit] == Switch_Pressed)
    {
        CommPublish(Edit_Switch);
    }

}
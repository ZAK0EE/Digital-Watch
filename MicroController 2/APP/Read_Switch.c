#include "../src/HAL/Switch/SWITCH.h"
#include "../lib/StdTypes.h"

void CommPublish(u8 Switch);

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

u8 Current_State [_Num_Of_Switches] = {0};
u8 Prev_State    [_Num_Of_Switches] = {0};
u8 State         [_Num_Of_Switches] = {0};  
u8 Counter       [_Num_Of_Switches] = {0};

void HSwitch(void)
{
    SWITCH_Get_State(INC_Switch , &Current_State[Inc]);
    SWITCH_Get_State(Mode_Switch, &Current_State[Mode]);
    SWITCH_Get_State(Edit_Switch, &Current_State[Edit]);

    for (u8 iter = 0; iter < _Num_Of_Switches; ++iter)
    {
        if(Current_State[iter] == Prev_State[iter])
        {
            ++Counter[iter];
        }

        if (Counter[iter] == SAMPLING_NUM)
        {
            State[iter] = Current_State[iter];
            Counter[iter] = 0;
        }

        Prev_State[iter] = Current_State[iter];

    }
    
}

void Read_Switch(void)
{
    if (State[Inc] == Switch_Pressed)
    {
        CommPublish(INC_SWITCH);
    }
    else
    if (State[Mode] == Switch_Pressed)
    {
        CommPublish(MODE_SWITCH);
    }
    else
    if (State[Edit] == Switch_Pressed)
    {
        CommPublish(Edit_Switch);
    }
    else
    {
        /*Do Nothing*/
    }
}
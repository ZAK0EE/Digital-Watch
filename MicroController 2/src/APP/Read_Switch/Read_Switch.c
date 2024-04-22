#include "../../HAL/Switch/SWITCH.h"
#include "../../lib/StdTypes.h"
#include "../Comm/Comm.h"
#include "../Frames_cfg.h"

#define INC_SWITCH   FRAME_SWITCH_NO1
#define MODE_SWITCH  FRAME_SWITCH_NO2
#define EDIT_SWITCH  FRAME_SWITCH_NO3
#define RESET_SWITCH FRAME_SWITCH_NO1_AND_NO2

#define SAMPLING_NUM 5

typedef enum
{
    Inc,
    Mode,
    Edit,
    //Reset,

    _Num_Of_Switches,
}Switches_t;

u32 Current_State = 0;
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
        Comm_publish(INC_SWITCH);
    }
    else
    if (State[Mode] == Switch_Pressed)
    {
        Comm_publish(MODE_SWITCH);
    }
    else
    if (State[Edit] == Switch_Pressed)
    {
        Comm_publish(EDIT_SWITCH);
    }

    // if (State[Reset] == Switch_Pressed)
    // {
    //     Comm_publish(RESET_SWITCH);
    // }
    

}
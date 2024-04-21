#include "HAL/LCD/LCD_Confg.h"
#include "MCAL/GPIO/GPIO.h"
//#include "SVC/Scheduler/Runnables.h"

#define DISPLAY_ON_CURSOR_OFF       0x0CUL          // display on, cursor off, don't blink character
#define DISPLAY_ON_CURSOR_ON        0x0EUL          // display on, cursor on, don't blink character
#define DISPLAY_OFF_CURSOR_OFF      0x08UL          // turn display off
#define CLEAR_DISPLAY               0x01UL          //replace all characters with ASCII 'space'
#define ENTRY_MODE                  0x06UL          // shift cursor from left to right on read/write
#define HOME                        0x02UL          // return cursor to first position on first line
#define CGRAM                       0x40UL          // the first Place/Address at CGRAM
#define SET_CURSOR                  0x80UL          // set cursor position
#define FUNCTION_SET                0x38UL          // reset the LCD


typedef enum
{
    LCD_Default,
    LCD_init,
    LCD_Operating,
    LCD_ShutDown,

}LCD_State_t;

typedef enum
{
    PowerON,
    Function_Set,
    Display_Control,
    Display_Clear,
    Entry_Mode,
    End,

}Init_State_t;

typedef enum
{
    Operation_Default,
    Write,
    Set_Cursor,
    Clear,
    Write_Number,

}Operating_Process_t;

typedef enum
{
    Ready,
    Busy,

}User_Req_State_t;

typedef struct
{
    u8 Type;
    u8 State;
    
}User_Req_t;

typedef struct
{
    LCD_Modules_t LCD_Name;
    const char *string;
    u8 String_Len;
    u8 String_Counter;

}Write_Req_t;

typedef struct 
{
    u8 Number;
    u8 LCD_Name;
}Number_Req_t;


typedef struct
{
    LCD_Modules_t LCD_Name;
    u8 Row;
    u8 Col;

}Set_Cursor_t;

typedef struct
{
    LCD_Modules_t LCD_Name;

}Clear_Req_t;


extern const LCD_PortPin_t LCD_Pins_Arr [LCD_Num];
u8 Time_mS = 0;
LCD_State_t LCD_State = LCD_init;
u8 Init_State = PowerON;
User_Req_t User_Req;
Set_Cursor_t Set_Cursor_Req;
Write_Req_t Write_Req;
Clear_Req_t Clear_Req;
Number_Req_t Number_Req;

/*Static Functions APIs*/
static void LCD_Send_Data(LCD_Modules_t LCD_Name ,u8 Data);
static void LCD_Send_Command(LCD_Modules_t LCD_Name, u8 Command);
static void LCD_Pins_Init(void);
static void LCD_Init_SM(void);
static void LCD_CLear_proc(void);
static void LCD_Set_Cursor_Proc(void);
static void LCD_Write_Proc(void);
static u32 Calc_string_Len(const char *string);
static void LCD_Operate(void);
static void LCD_Write_Number_Proc(void);
static void LCD_Send_Number(LCD_Modules_t LCD_Name ,u8 Data);

void LCD_Task(void)
{
    switch (LCD_State)
    {
    case LCD_init:
        LCD_Init_SM();
        break;

    case LCD_Operating:
        LCD_Operate();
        break;

    case LCD_ShutDown:
        break;
    
    default:
        break;
    }
}


/*LCD State Machine Init*/
static void LCD_Init_SM(void)
{
    
    Time_mS += 1;

    switch (Init_State)
    {
    case PowerON:
        if (Time_mS == 30)
        {
        LCD_Pins_Init();
        Init_State = Function_Set;
        Time_mS = 0;
        }
        break;

    case Function_Set:
        if (Time_mS == 1)
        {
        for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, FUNCTION_SET);
        }
        }
        else if (Time_mS > 1)
        {
            for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, FUNCTION_SET);
        }
            Init_State = Display_Control;
            Time_mS = 0;
        }
        break;

    case Display_Control:
        if (Time_mS == 1)
        {
        for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, 0x0f);
        }
        }
        else if (Time_mS > 1)
        {
            for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, 0x0f);
        }
            Init_State = Display_Clear;
            Time_mS = 0;
        }
        break;

    case Display_Clear:
        if (Time_mS == 1)
        {
        for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, CLEAR_DISPLAY);
        }
        }
        else if (Time_mS > 1)
        {
            for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, CLEAR_DISPLAY);
        }
            Init_State = Entry_Mode;
            Time_mS = 0;
        }
        break;

    case Entry_Mode:
        if (Time_mS == 1)
        {
        for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, ENTRY_MODE);
        }
        }
        else if (Time_mS > 1)
        {
            for (u8 Counter = 0; Counter < LCD_Num; ++Counter)
        {
            LCD_Send_Command(Counter, ENTRY_MODE);
        }
            Init_State = End;
            Time_mS = 0;
        }
        break;

    case End:
        LCD_State = LCD_Operating;
        Time_mS = 0;
        break;

    
    default:
        break;
    }
}


static void LCD_Operate(void)
{
    Time_mS += 1;

    switch (User_Req.Type)
    {
    case Write:
        if (Write_Req.String_Len)
        {
            LCD_Write_Proc();
        }
        else
        {
            User_Req.State = Ready;
            User_Req.Type=LCD_Default;
            Write_Req.String_Counter = 0;
            Write_Req.String_Len = 0; 
            Time_mS = 0;
        }
        break;

    case Set_Cursor:
        if (Time_mS == 1)
        {
            LCD_Set_Cursor_Proc();
        }
        else if (Time_mS == 2)
        {
            LCD_Set_Cursor_Proc();
            User_Req.State = Ready;
            User_Req.Type=LCD_Default;
            Time_mS = 0;
        }
        break;

    case Clear:
        if (Time_mS == 1)
        {
            LCD_CLear_proc();
        }
        else if (Time_mS == 2)
        {
            LCD_CLear_proc();
        }
        else if (Time_mS == 4)
        {
            User_Req.State = Ready;
            User_Req.Type=LCD_Default;
            Time_mS = 0;
        }
        break;
    case Write_Number:
        if (Time_mS == 1)
        {
            LCD_Write_Number_Proc();
        }
        else if (Time_mS == 2)
        {
            LCD_Write_Number_Proc();
            User_Req.State = Ready;
            User_Req.Type=LCD_Default;
            Time_mS = 0;
        }
        break;
        
    default:
    Time_mS = 0;
        break;
    }
}


void LCD_Clear_Async(LCD_Modules_t LCD_Name)
{
    if (LCD_State == LCD_Operating && User_Req.State == Ready)
    {
        User_Req.State = Busy;
        User_Req.Type  = Clear;

        Clear_Req.LCD_Name = LCD_Name;
    }
    
}

static void LCD_CLear_proc(void)
{
    LCD_Send_Command(Clear_Req.LCD_Name, CLEAR_DISPLAY);
}


void LCD_Set_Cursor_Async(LCD_Modules_t LCD_Name ,u8 Row, u8 Col)
{
    if (LCD_State == LCD_Operating && User_Req.State == Ready)
    {
        User_Req.State = Busy;
        User_Req.Type  = Set_Cursor;

        Set_Cursor_Req.LCD_Name = LCD_Name;
        Set_Cursor_Req.Row      = Row;
        Set_Cursor_Req.Col      = Col;
    }
    
}


static void LCD_Set_Cursor_Proc(void)
{
    u8 Temp = 0;
    if(Time_mS==1)
    {
    if (Set_Cursor_Req.Row<1 || Set_Cursor_Req.Row>2 || Set_Cursor_Req.Col<1 || Set_Cursor_Req.Col>16)
    {
        Temp = SET_CURSOR;
    }
    else if (Set_Cursor_Req.Row == 1)
    {
        Temp = SET_CURSOR + (Set_Cursor_Req.Col-1);
    }
    else if (Set_Cursor_Req.Row == 2)
    {
        Temp = (SET_CURSOR + 0x40) + (Set_Cursor_Req.Col-1);
    }
    }

    LCD_Send_Command(Set_Cursor_Req.LCD_Name, Temp);
    
}



void LCD_Write_String_Async(LCD_Modules_t LCD_Name ,const char *string)
{

    if (LCD_State == LCD_Operating && User_Req.State == Ready)
    {
        User_Req.State      = Busy;
        User_Req.Type       = Write;

        Write_Req.string     = string; 
        Write_Req.LCD_Name   = LCD_Name;
        Write_Req.String_Len = Calc_string_Len(string);
    }
    
}

static void LCD_Write_Proc(void)
{
    LCD_Send_Data(Write_Req.LCD_Name, Write_Req.string[Write_Req.String_Counter]);
}


void LCD_Write_NUM(LCD_Modules_t LCD_Name, u8 Number)
{
    if (LCD_State == LCD_Operating && User_Req.State == Ready)
    {
        User_Req.State      = Busy;
        User_Req.Type       = Write_Number;

        Number_Req.Number = Number;
        Number_Req.LCD_Name = LCD_Name;
    }
}

static void LCD_Write_Number_Proc(void)
{
    LCD_Send_Number(Number_Req.LCD_Name, Number_Req.Number+48);
}

u8 LCD_Get_State(void)
{
    return User_Req.State;
}


static void LCD_Send_Number(LCD_Modules_t LCD_Name ,u8 Data)
{
    u8 Counter2 = 0;

    if (Time_mS==1)
    {

        for (u8 Counter1 = LCD_DB0; Counter1 < LCD_Pins_Num; ++Counter1)
        { 
            GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[Counter1].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[Counter1].Pin, ((Data >> Counter2++) & 1UL ) );
        } 
        
    /*RS=1*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RS].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RS].Pin, GPIO_PIN_ON);
    /*RW=0*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RW].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RW].Pin, GPIO_PIN_OFF);
    /*E=1*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Pin, GPIO_PIN_ON);

    }
    
    else
    {
        /*E=0*/
        GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Pin, GPIO_PIN_OFF);
    }
    
}


/*Send data*/
static void LCD_Send_Data(LCD_Modules_t LCD_Name ,u8 Data)
{
    u8 Counter2 = 0;

    if (Time_mS%2)
    {

        for (u8 Counter1 = LCD_DB0; Counter1 < LCD_Pins_Num; ++Counter1)
        { 
            GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[Counter1].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[Counter1].Pin, ((Data >> Counter2++) & 1UL ) );
        } 
        
    /*RS=1*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RS].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RS].Pin, GPIO_PIN_ON);
    /*RW=0*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RW].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RW].Pin, GPIO_PIN_OFF);
    /*E=1*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Pin, GPIO_PIN_ON);

    }
    
    else
    {
        /*E=0*/
        GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Pin, GPIO_PIN_OFF);
        
        ++Write_Req.String_Counter;
        --Write_Req.String_Len;
    }
    
}

/*Send Command*/
static void LCD_Send_Command(LCD_Modules_t LCD_Name, u8 Command)
{
    u8 Counter2 = 0;

    if (Time_mS == 1)
    {
        for (u8 Counter1 = LCD_DB0; Counter1 < LCD_Pins_Num; ++Counter1)
        { 
            GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[Counter1].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[Counter1].Pin, ((Command >> Counter2++) & 1 ) );
        } 
        
    /*RS=0*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RS].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RS].Pin, GPIO_PIN_OFF);
    /*RW=0*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RW].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_RW].Pin, GPIO_PIN_OFF);
    /*E=1*/
    GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Pin, GPIO_PIN_ON);

    }
    else if (Time_mS > 1)
    {
        /*E=0*/
        GPIO_Set_Pin_State(LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Port, LCD_Pins_Arr[LCD_Name].LCD_arr[LCD_E].Pin, GPIO_PIN_OFF);
    }

    }

/*LCD Pins Init*/
static void LCD_Pins_Init(void)
{
    u8 Counter1 = 0, Counter2 = 0;

    GPIO_Pin_Confg_t LCD_Pin;

    LCD_Pin.mode  = GPIO_CONFG_OUTPUT_PP;
    LCD_Pin.speed = GPIO_PIN_MID_SPEED;

    for(Counter1 = 0; Counter1<LCD_Num; ++Counter1)
    {
        for(Counter2 = 0; Counter2<LCD_Pins_Num; ++Counter2)
        {
            LCD_Pin.port = LCD_Pins_Arr[Counter1].LCD_arr[Counter2].Port;
            LCD_Pin.pin  = LCD_Pins_Arr[Counter1].LCD_arr[Counter2].Pin;

            GPIO_Init_Pin(&LCD_Pin);
        }
    }
}



static u32 Calc_string_Len(const char *string)
{
    const char *ptr = string;

    while (*ptr)
    {
        ++ptr;
    }

    return (ptr - string);
}
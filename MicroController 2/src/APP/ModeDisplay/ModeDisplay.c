#include "../../lib/StdTypes.h"
#include "../HAL/LCD/LCD.h"
#include "../Mode/Mode.h"
#include "../DateAndTime/DateAndTime.h"
#include "../StopWatch/StopWatch.h"
#include "../Mode/Mode.h"

// typedef struct
// {
//     // H1H2 : M1M2 : S1S2
//     u8 H1;
//     u8 H2;

//     u8 M1;
//     u8 M2;

//     u8 S1;
//     u8 S2;

//     u8 Ms1;
//     u8 Ms2;

// }Time_t;

// typedef struct
// {
//     //D1D2 : M1M2 : Y1Y2Y3Y4
//     u8 D1;
//     u8 D2;

//     u8 M1;
//     u8 M2;

//     u8 Y1;
//     u8 Y2;
//     u8 Y3;
//     u8 Y4;

// }Date_t;


// typedef enum
// {
//     H1,
//     H2,
//     M1,
//     M2,
//     S1,
//     S2,

//     D1,
//     D2,
//     Mo1,
//     Mo2,
//     Y1,
//     Y2,
//     Y3,
//     Y4,
// }Digits_t;

u8 TimeMs  = 0;

/*static APIs*/
static void Print_Current_Date_And_Time(Date_t Date, Time_t Time);
static void print_Stop_Watch(Time_t Time);
static void Print_Edit_Date_And_Time(u8 Current_Digit);

void ModeDisplay(void)
{   
    u8 Current_Mode = getMode();
    Time_t Time = getTime();
    u8 Current_Digit = getCurrentDigit();

    switch (Current_Mode)
    {
    case Mode_DateAndTime:
        Print_Current_Date_And_Time(Date, Time);
        break;
    case Mode_StopWatch:
        print_Stop_Watch(Time);
        break;
    case Mode_EditDateAndTime:
        Print_Edit_Date_And_Time(Current_Digit);
        break;
    
    default:
        break;
    }
}

static void Print_Current_Date_And_Time(Date_t Date, Time_t Time)
{
    ++TimeMs;

    switch (TimeMs)
    {
    case 1:
        LCD_Set_Cursor_Async(LCD1, 2, 3);
        break;
    case 2:
       LCD_Write_NUM(LCD1, Date.D1);
        break;
    case 3:
        LCD_Set_Cursor_Async(LCD1, 2, 4);
        break;
    case 4:
        LCD_Write_NUM(LCD1, Date.D2);
        break;
    case 5:
        LCD_Set_Cursor_Async(LCD1, 2, 5);
        break;
    case 6:
        LCD_Write_String_Async(LCD1, "/");
        break;
    case 7:
        LCD_Set_Cursor_Async(LCD1, 2, 6);
        break;
    case 8:
        LCD_Write_NUM(LCD1, Date.M1);
        break;
    case 9:
        LCD_Set_Cursor_Async(LCD1, 2, 7);
        break;
    case 10:
        LCD_Write_NUM(LCD1, Date.M2);
        break;
    case 11:
        LCD_Set_Cursor_Async(LCD1, 2, 8);
        break;
    case 12:
        LCD_Write_String_Async(LCD1, "/");
        break;
    case 13:
        LCD_Set_Cursor_Async(LCD1, 2, 9);
        break;
    case 14:
        LCD_Write_NUM(LCD1, Date.Y1);
        break;
    case 15:
        LCD_Set_Cursor_Async(LCD1, 2, 10);
        break;
    case 16:
        LCD_Write_NUM(LCD1, Date.Y2);
        break;
    case 17:
        LCD_Set_Cursor_Async(LCD1, 2, 11);
        break;
    case 18:
        LCD_Write_NUM(LCD1, Date.Y3);
        break;
    case 19:
        LCD_Set_Cursor_Async(LCD1, 2, 12);
    case 20:
        LCD_Write_NUM(LCD1, Date.Y4);
        break;
    case 21:
        LCD_Set_Cursor_Async(LCD1, 1, 3);
        break;
    case 22:
       LCD_Write_NUM(LCD1, Time.H1);
        break;
    case 23:
        LCD_Set_Cursor_Async(LCD1, 1, 4);
        break;
    case 24:
        LCD_Write_NUM(LCD1, Time.H2);
        break;
    case 25:
        LCD_Set_Cursor_Async(LCD1, 1, 5);
        break;
    case 26:
        LCD_Write_String_Async(LCD1, ":");
        break;
    case 27:
        LCD_Set_Cursor_Async(LCD1, 1, 6);
        break;
    case 28:
        LCD_Write_NUM(LCD1, Time.M1);
        break;
    case 29:
        LCD_Set_Cursor_Async(LCD1, 1, 7);
        break;
    case 30:
        LCD_Write_NUM(LCD1, Time.M2);
        break;
    case 31:
        LCD_Set_Cursor_Async(LCD1, 1, 8);
        break;
    case 32:
        LCD_Write_String_Async(LCD1, ":");
        break;
    case 33:
        LCD_Set_Cursor_Async(LCD1, 1, 9);
        break;
    case 34:
        LCD_Write_NUM(LCD1, Time.S1);
        break;
    case 35:
        LCD_Set_Cursor_Async(LCD1, 1, 10);
        break;
    case 36:
        LCD_Write_NUM(LCD1, Time.S2);
        break;
    case 37:
        TimeMs = 0;
        break;

    default:
    TimeMs = 0;
        break;
    }
    
    
}

static void print_Stop_Watch(Time_t Time)
{
    ++TimeMs;
    switch (TimeMs)
    {
     case 1:
        LCD_Set_Cursor_Async(LCD1, 1, 3);
        break;
    case 2:
       LCD_Write_NUM(LCD1, Time.H1);
        break;
    case 3:
        LCD_Set_Cursor_Async(LCD1, 1, 4);
        break;
    case 4:
        LCD_Write_NUM(LCD1, Time.H2);
        break;
    case 5:
        LCD_Set_Cursor_Async(LCD1, 1, 5);
        break;
    case 6:
        LCD_Write_String_Async(LCD1, ":");
        break;
    case 7:
        LCD_Set_Cursor_Async(LCD1, 1, 6);
        break;
    case 8:
        LCD_Write_NUM(LCD1, Time.M1);
        break;
    case 9:
        LCD_Set_Cursor_Async(LCD1, 1, 7);
        break;
    case 10:
        LCD_Write_NUM(LCD1, Time.M2);
        break;
    case 11:
        LCD_Set_Cursor_Async(LCD1, 1, 8);
        break;
    case 12:
        LCD_Write_String_Async(LCD1, ":");
        break;
    case 13:
        LCD_Set_Cursor_Async(LCD1, 1, 9);
        break;
    case 14:
        LCD_Write_NUM(LCD1, Time.S1);
        break;
    case 15:
        LCD_Set_Cursor_Async(LCD1, 1, 10);
        break;
    case 16:
        LCD_Write_NUM(LCD1, Time.S2);
        break;
    case 17:
        LCD_Set_Cursor_Async(LCD1, 1, 11);
        break;
    case 18:
        LCD_Write_String_Async(LCD1,":");
        break;
    case 19:
        LCD_Set_Cursor_Async(LCD1,1,12);
        break;
    case 20:
        LCD_Write_NUM(LCD1, Time.Ms1);
        break;
    case 21:
        LCD_Set_Cursor_Async(LCD1,1,13);
        break;
    case 22:
        LCD_Write_NUM(LCD1, Time.Ms2);
        break;
    case 23:
        TimeMs = 0;
        break;
    default:
        TimeMs=0;
        break;
    }
}

static void Print_Edit_Date_And_Time(u8 Current_Digit)
{
    //Print_Current_Date_And_Time(Date, Time);

    switch (Current_Digit)
    {
    case H1:
        LCD_Set_Cursor_Async(LCD1, 1, 3);
        break;
    case H2:
        LCD_Set_Cursor_Async(LCD1, 1, 4);
        break;
    case M1:
        LCD_Set_Cursor_Async(LCD1, 1, 6);
        break;
    case M2:
        LCD_Set_Cursor_Async(LCD1, 1, 7);
        break;
    case S1:
        LCD_Set_Cursor_Async(LCD1, 1, 9);
        break;
    case S2:
        LCD_Set_Cursor_Async(LCD1, 1, 10);
    
    
    case D1:
        LCD_Set_Cursor_Async(LCD1, 2, 3);
        break;
    case D2:
        LCD_Set_Cursor_Async(LCD1, 2, 4);
        break;
    case Mo1:
        LCD_Set_Cursor_Async(LCD1, 2, 6);
        break;
    case Mo2:
        LCD_Set_Cursor_Async(LCD1, 2, 7);
        break;
    case Y1:
        LCD_Set_Cursor_Async(LCD1, 2, 9);
        break;
    case Y2:
        LCD_Set_Cursor_Async(LCD1, 2, 10);
        break;
    case Y3:
        LCD_Set_Cursor_Async(LCD1, 2, 11);
        break;
    case Y4:
        LCD_Set_Cursor_Async(LCD1, 2, 12);
        break;
    
    default:
        LCD_Set_Cursor_Async(LCD1, 1, 1);
        break;
    }
    
}
/**
 * @file Scheduler_cfg.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Scheduler.h"
#include "Scheduler_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern void Display_task(void);
extern void LCD_task(void);
extern void Clock_TEST_TASK (void) ;
extern void display_testa(void) ;
extern void Stopwatch_test(void);
extern void DW_Runnable(void) ;
extern void HSwitch_Runnable(void);
extern void Button_task(void);
extern void HUART_Runnable(void);

Sched_Runnable_Config_t Sched_Runnables[_NUM_OF_RUNNABLES] = 
{

     /* Example*/
     /*
     [SCHED_TRAFFICLIGHT]=
     {
          .CallBack = TrafficLight_task,
          .DelayMS = 0,
          .PeriodicityMS = 1000,
     },    
     */
     [SCHED_DigitalWatch_Task]=
     {
          .CallBack = DW_Runnable,
          .DelayMS = 100,
          .PeriodicityMS = 100,
     },    
     [SCHED_LCD_TASK]=
     {
          .CallBack = LCD_task,
          .DelayMS = 0,
          .PeriodicityMS = 1,
     },
     [SCHED_DISPLAY_TASK]=
     {
          .CallBack = Display_task,
          .DelayMS = 20,
          .PeriodicityMS = 5,
     },
     [SCHED_SWITCH_TASK]=
     {
          .CallBack = HSwitch_Runnable,
          .DelayMS = 40,
          .PeriodicityMS = 5,
     },
     [SCHED_BUTTON_TASK]=
     {
          .CallBack = Button_task,
          .DelayMS = 40,
          .PeriodicityMS = 100,
     },
};
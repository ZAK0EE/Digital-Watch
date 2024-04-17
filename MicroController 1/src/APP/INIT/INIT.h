/*
 ============================================================================
 Name        : INIT.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the INIT Driver
 Date        : 15/4/2024
 ============================================================================
 */
#ifndef INIT_H_
#define INIT_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "LIB/std_types.h"
#include "LIB/Mask32.h"
#include "LIB/Error.h"
#include "Services/Scheduler/Scheduler.h"
#include "Services/Scheduler/Scheduler_cfg.h"
#include "HAL/HUART/HUART.h"
#include "HAL/HSwitch/HSwitch.h"
#include "HAL/LCD/LCD.h"
#include "APP/Display/Display.h"
#include "HAL/ControlClock/CLK_Control.h"
#include "APP/Button/Button.h"
/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/

/**
 * @brief    : Initializes the system components.
 * @details  : This function initializes the GPIO clock for GPIOA, initializes the hardware UART (HUART), 
 *             initializes the hardware switch (HSwitch), initializes the LCD (LCD1), and initializes the display.
 * @param    : None
 * @return   : None
 **/
void InitSys (void) ;



#endif
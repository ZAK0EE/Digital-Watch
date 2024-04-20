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
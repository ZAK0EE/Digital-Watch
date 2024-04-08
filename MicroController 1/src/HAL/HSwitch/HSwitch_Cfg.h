/*
 ============================================================================
 Name        : STK_Cfg.h
 Author      : Omar Medhat Mohamed
 Description : Header Config File for the HSwitch Driver (Solve Debounce problem)
 Date        : 15/3/2024
 ============================================================================
 */
#ifndef HSwitch_CFG_H_
#define HSwitch_CFG_H_
/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
/* Configure The HSwitchs Namein this Enum */
typedef enum 
{
	/* EX : HSwitch */
	HSwitch1,
	HSwitch2,
	/*Indicate number of HSwitchd don't use it */
    _HSwitch_Num 
}HSwitchS_t;

#endif



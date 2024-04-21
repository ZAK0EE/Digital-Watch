#ifndef DateAndTime_H_
#define DateAndTime_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "StdTypes.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

// Structure definition for time
typedef struct {
    u8 hours;      // Hours (0-23)
    u8 minutes;    // Minutes (0-59)
    u8 seconds;    // Seconds (0-59)
    u8 milliseconds; // Milliseconds (0-99)
} Time;

// Structure definition for date
typedef struct {
    u8 day;        // Day of the month (1-31)
    u8 month;      // Month (1-12)
    u16 year;      // Year
} Date;

// Define an enum for the digits of date and time
typedef enum {
    DIGIT_HOURS,
    DIGIT_MINUTES,
    DIGIT_SECONDS,
    DIGIT_DAY,  // Adjusted to come before DIGIT_MONTH
    DIGIT_MONTH,
    DIGIT_YEAR_THOUSANDS,
    DIGIT_YEAR_HUNDREDS,
    DIGIT_YEAR_TENS,
    DIGIT_YEAR_UNITS
} DateTimeDigit;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

// API function to edit date and time
void edit_DateTime(u8 frame);

// Getter API for time
Time getTime(void);

// Getter API for date
Date getDate(void);

// Getter API for current digit being edited
DateTimeDigit getCurrentDigit(void);


#endif // DateAndTime_H_
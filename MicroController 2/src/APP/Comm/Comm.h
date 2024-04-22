#ifndef Comm_H_
#define Comm_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "../../lib/StdTypes.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef enum{
    Comm_Ok,
    Comm_Nok,
    Comm_InvalidRxFrame,
    Comm_InvalidTxFrame
}Comm_errorStatus_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

Comm_errorStatus_t Comm_publish(u8 frame);
void Comm_collect(void);

#endif // Comm_H_
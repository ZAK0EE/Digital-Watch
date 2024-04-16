#include "Error.h"
#include "../StdTypes.h"

Error_t Validate_NULL_Pointer(void *Ptr)
{
    Error_t Error_State;

    if (Ptr)
    {
        Error_State = ERROR_Ok;
    }
    else
    {
        Error_State = ERROR_NULL_Pointer;
    }
    
    return Error_State;
}
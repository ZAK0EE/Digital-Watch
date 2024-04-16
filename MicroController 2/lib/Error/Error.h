#ifndef ERROR_H_
#define ERROR_H_

typedef enum
{

    ERROR_Nok,
    ERROR_Ok,
    ERROR_Invalid_Argument,
    Error_Out_Of_Range,
    ERROR_Invalid_IRQ,
    ERROR_NULL_Pointer,

}Error_t;

Error_t Validate_NULL_Pointer(void *Ptr);

#endif /*ERROR_H_*/
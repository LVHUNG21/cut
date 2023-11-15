/******************************************************************************
 * Include
 *****************************************************************************/
#include "srec.h"
#ifndef _IO_H_
#define _IO_H_ 
extern uint32_t line_error[100];
extern uint32_t lineError;
/**
 * @brief     print messge to user 
 * @param[in] status 
 * @return    Message 
 */
char * IO_Message(SREC_Status_Type status);
#endif /* _IO_H_H */
/******************************************************************************
 * End of file
 ******************************************************************************/
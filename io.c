/******************************************************************************
 * Include
 *****************************************************************************/
#include "io.h"
#include "string.h"

static char *Message[] = {
    "\033[32m SREC LINE TRUE \033[0m",
    "SREC BYTECOUNT OK",
    "\033[31m SREC ERROR FORMAT \033[0m",
    "\033[31m SREC ERROR BYTECOUNT \033[0m",
    "\033[31m SREC ERROR CHECKSUM \033[0m",
    "\033[31m SREC ERROR FILE DOESN'T EXST \033[0m",
    "\033[31m SREC ERROR GREATER BYTECOUNT \033[0m",
    "\033[31m SREC ERROR LESS THAN BYTECOUNT \033[0m",
    "\033[31m SREC INVALID TYPE \033[0m",
    "\033[31m SREC TYPE HEAD \033[0m",
    "SREC TYPE CONTAIN DATA",
    "\033[31m SREC TYPE COUNT (S5 S6) \033[0m",
    "\033[31m SREC TYPE END FILE (S9 S8 S7) \033[0m",
    "SREC CHECKSUM OK",
    "\033[31m SREC ERROR HAVE SPACE IN LINE \033[0m",
    "SREC READ ADDRESS OK",
    "\033[31m SREC_ERROR_FORMAT(First char must be 'S')  \033[0m",
    "\033[31m SREC_ERROR_FORMAT(The character aren't in hexa format) \033[0m",
    "SREC CHARACTER OK",
    "\033[31m SREC HEADER ERROR (Type of header must be '0'(S0) and not an empty line) \033[0m",
    "\033[31m This line is header \033[0m",
    "Convert string ok",
    "\033[31m APPEAR EMPTY LINE IN FILE(STOP PARSE) \033[0m",
    "\033[1M \n \033[34m RESULT:THE FILE OK(NOT ERROR) \033[0m",
    "\033[34mRESULT: FILE APPEAR ERROR \033[0m",
    "\033[31mSREC eror character(upper case)\033[0m"};

/******************************************************************************
 * Funtion
 ******************************************************************************/
char *IO_Message(SREC_Status_Type status)
{
    if (status == SREC_FILE_ERROR)
    {
        // printf("\033[1M \n \033[34mRESULT: FILE APPEAR ERROR \033[0m");
        printf("\n\033[34m(Line errors: ");
        for (int i = 0; i < 100; i++)
        {
            if (line_error[i] != 0)
            {
                printf("%d ", line_error[i]);
            }
        }
        printf(")\033[0m \n");
    }
    return Message[status];
}

/******************************************************************************
 * End of file
 ******************************************************************************/

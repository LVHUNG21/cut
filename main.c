/******************************************************************************
 * Include
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "srec.h"
#include "io.h"

#define length_buffer 100
/******************************************************************************
 * Main function
 *****************************************************************************/
int main()
{
    SREC_Status_Type status = read_file_srec("blinkLed.srec");
    if (status == 5)
    {
        char *message = IO_Message(status);
        printf("%s\n", message);
    }
    else
    {
        char *message = IO_Message(status);
        printf("%s\n", message);
    }
    return 0;
}
/******************************************************************************
 * End of file
 ******************************************************************************/

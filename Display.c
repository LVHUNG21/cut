/******************************************************************************
 * Include
 *****************************************************************************/
#include "io.h"
#include "Display.h"

/******************************************************************************
 * Prototypes
 *****************************************************************************/
static Display_Status_Type printData(uint8_t *buf, uint8_t size);

/******************************************************************************
 * Function
 *****************************************************************************/
Display_Status_Type displaySRECLine(uint32_t lineNum, uint32_t address, uint8_t *data, uint8_t size, uint32_t *result_and_type)
{
    printf("\n----- LINE %02d -----\n", lineNum);
    if (result_and_type[0] == 0)
    {
        if (result_and_type[1] == 1)
        {
            printf("Address: 0x%04X \n", address);
        }
        else if (result_and_type[1] == 2)
        {
            printf("Address: 0x%06X \n", address);
        }
        else if (result_and_type[1] == 3)
        {
            printf("Address: 0x%08X \n", address);
        }
        printf("Data: ");
        printData(data, size);
        char *mess = IO_Message(result_and_type[0]);
        printf("Result of this line :%s\n", mess);
    }
    else
    {
        char *message = IO_Message(result_and_type[0]);
        printf("%s\n", message);
    }
};
static Display_Status_Type printData(uint8_t *data, uint8_t size)
{

    if (size == 0)
    {
        printf(" Please try again  ");
    }
    for (size_t i = 0; i < size; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
}
/******************************************************************************
 * End of file
 ******************************************************************************/
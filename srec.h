/******************************************************************************
 * Include
 *****************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "Display.h"

#ifndef _SREC_H_
#define _SREC_H_

typedef enum
{
    SREC_LINE_TRUE = 0,
    SREC_BYTECOUNT_OK,
    SREC_ERROR_FORMAT,
    SREC_ERROR_BYTECOUNT,
    SREC_ERROR_CHECKSUM,
    SREC_ERROR_FILE_EXIST,
    SREC_ERROR_GREATER_BYTECOUNT,
    SREC_ERROR_LESS_BYTECOUNT,
    SREC_INVALID_TYPE,
    SREC_TYPE_HEAD,
    SREC_TYPE_CONTAIN_DATA,
    SREC_TYPE_COUNT_S5_S6,
    SREC_TYPE_END_FILE_S9_S8_S7,
    SREC_CHECKSUM_OK,
    SREC_SPACE_IN_LINE,
    SREC_READ_ADDRESS_OK,
    SREC_ERROR_FIRST_CHARACTER,
    SREC_ERROR_CHARACTER,
    SREC_CHARACTER_OK,
    SREC_HEADER_ERROR,
    SREC_HEADER,
    SREC_CONVERT_STRING_OK,
    SREC_ERROR_EMPTY_LINE,
    SREC_PARSED,
    SREC_FILE_ERROR,
    SREC_ERROR_CHARACTER2,
} SREC_Status_Type;

typedef struct
{
    uint8_t SrecType[3];
    uint8_t ByteCount;
    uint32_t Address;
    uint8_t *Data; // Point to heap (storage Data)
    uint8_t Checksum;
} SREC_Type;

/******************************************************************************
 * API
 *****************************************************************************/
SREC_Status_Type read_file_srec(char * filename);

#endif /* _SREC_H */
/******************************************************************************
 * End of file
 ******************************************************************************/
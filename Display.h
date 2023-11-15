/******************************************************************************
 * Include
 *****************************************************************************/
#include<stdint.h>
#ifndef _DISPLAY_H 
#define _DISPLAY_H

typedef enum {
    DISPLAY_SUCCESS,
    DISPLAY_INVALID_PARAMETER
} Display_Status_Type;

/******************************************************************************
 * API
 *****************************************************************************/

/**
 * @brief Print line from Srec file
 * 
 * @param[in] lineNum 
 * @param[in] address 
 * @param[in] data 
 * @param[in] size 
 * @param[in] result_and_type 
 * @return    Display_Status_Type 
 */
Display_Status_Type displaySRECLine(uint32_t lineNum, uint32_t address, uint8_t *data, uint8_t size, uint32_t* result_and_type);

#endif 
/******************************************************************************
 * End of file
 ******************************************************************************/
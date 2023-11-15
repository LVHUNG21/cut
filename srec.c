/******************************************************************************
 * Include
 *****************************************************************************/
#include "srec.h"

/******************************************************************************
 * Global variable
 *****************************************************************************/
static SREC_Type SrecData;
static uint8_t AddressNum = 0;
static uint8_t DataBytes = 0;
static FILE *file = NULL;
uint32_t line_error[100] = {0};
uint32_t lineError = 0;

#define HEX_INVALID 0XFF
/******************************************************************************
 * Prototypes
 *****************************************************************************/

/**
 * @brief Convert to hexa
 * @param hex
 * @return uint8_t
 */
static uint8_t convert_hex(char hex);

/**
 * @brief Convert string to hex
 *
 * @param pStr
 * @param len
 * @param pResult
 * @return SREC_Status_Type
 */
static SREC_Status_Type str_to_hex(char *pStr, uint8_t len, uint32_t *pResult);

/**
 * @brief Check character on the line of the file
 * @param line_check
 * @param line_number
 * @return SREC_Status_Type
 */
static SREC_Status_Type Check_character(uint8_t *line_check, uint32_t line_number);

/**
 * @brief Calculate and check check sum on the line
 * @param line
 * @return SREC_Status_Type
 */
static SREC_Status_Type Check_CheckSum(uint8_t *line);

/**
 * @brief Read file data on the line to element data of struct
 * @param line
 * @param size_field_data
 * @return SREC_Status_Type
 */
static SREC_Status_Type SREC_readData(uint8_t *line, uint8_t *size_field_data);

/**
 * @brief Get and check bytecount on the line
 * @param line
 * @return SREC_Status_Type
 */
static SREC_Status_Type SREC_GetAndCheckByteCount(uint8_t *line);

/**
 * @brief Check type srec
 * @param line
 * @return SREC_Status_Type
 */
static SREC_Status_Type SREC_CheckTypeSrec(uint8_t *line);

/**
 * @brief read address on the line to element address of struc
 * @param line
 * @return SREC_Status_Type
 */
static SREC_Status_Type SREC_readAddress(uint8_t *line);

/**
 * @brief Parse one line of the srec file
 * @param lineNum
 * @param line
 * @param size_field_data
 * @return SREC_Status_Type
 */
static SREC_Status_Type parse_line(uint32_t lineNum, uint8_t *line, uint8_t *size_field_data);

/**
 * @brief Parse file srec
 * @return SREC_Status_Type
 */
static SREC_Status_Type parse_file();

/**
 * @brief close file and free data in struct
 * @return SREC_Status_Type
 */
static SREC_Status_Type close_and_free();

/******************************************************************************
 * Function
 *****************************************************************************/
SREC_Status_Type read_file_srec(char *filename)
{
	SREC_Status_Type status = SREC_PARSED;
	file = fopen(filename, "r");
	if (file == NULL)
	{
		status = SREC_ERROR_FILE_EXIST;
	}
	else
	{
		status = parse_file();
	}
	close_and_free();
	return status;
}
static SREC_Status_Type close_and_free()
{
	free(SrecData.Data);
	fclose(file);
}
static SREC_Status_Type parse_file()
{
	SREC_Status_Type status = SREC_PARSED;
	uint32_t lineNum = 0;
	uint8_t line[100];
	/* variable to exit loop while */
	uint8_t exit_loop = 1;
	/* store result and type to print */
	uint32_t result_and_type[2];
	/* size of data to print */
	uint8_t size_field_data = 0;
	/* flag check file error or not */
	uint8_t flag_status = 'y';
	/* array store line error */

	while (fgets(line, sizeof(line), file) != NULL && exit_loop == 1)
	{
		lineNum++;
		status = parse_line(lineNum, line, &size_field_data);
		if ((status == SREC_ERROR_BYTECOUNT) || (status == SREC_ERROR_CHARACTER) ||
			(status == SREC_ERROR_CHECKSUM) || (status == SREC_ERROR_EMPTY_LINE) ||
			(status == SREC_ERROR_FIRST_CHARACTER) || (status == SREC_ERROR_GREATER_BYTECOUNT) ||
			(status == SREC_ERROR_LESS_BYTECOUNT) || (status == SREC_HEADER_ERROR) ||
			(status == SREC_ERROR_FORMAT) || (status == SREC_INVALID_TYPE) || (status == SREC_SPACE_IN_LINE))
		{
			line_error[lineError] = lineNum;
			lineError++;
			flag_status = 'n';
		}
		/* check if header error and appear empty line in file (STOP PARSE) */
		if (status == SREC_HEADER_ERROR || status == SREC_ERROR_EMPTY_LINE)
		{
			result_and_type[0] = status;
			displaySRECLine(lineNum, SrecData.Address, SrecData.Data, size_field_data, result_and_type);
			/* set exit_loop to 0 to exit loop */
			exit_loop = 0;
		}
		else
		{
			result_and_type[0] = status;
			result_and_type[1] = convert_hex(line[1]);
			displaySRECLine(lineNum, SrecData.Address, SrecData.Data, size_field_data, result_and_type);
		}
	}
	if (flag_status == 'n')
	{
		status = SREC_FILE_ERROR;
	}
	else
	{
		status = SREC_PARSED;
	}
	return status;
}
static SREC_Status_Type parse_line(uint32_t lineNum, uint8_t *line, uint8_t *size_field_data)
{
	SREC_Status_Type status = SREC_LINE_TRUE;
	uint8_t data[60];
	uint8_t return_or_not = 0;

	/* check all character must be hexa */
	if (Check_character(line, lineNum) != SREC_CHARACTER_OK)
	{
		status = Check_character(line,lineNum);
		return_or_not = 1;
	}

	if (!return_or_not && SREC_CheckTypeSrec(line) != SREC_TYPE_CONTAIN_DATA)
	{
		status = SREC_CheckTypeSrec(line);
		return_or_not = 1;
	}

	if (!return_or_not && SREC_GetAndCheckByteCount(line) != SREC_BYTECOUNT_OK)
	{
		status = SREC_GetAndCheckByteCount(line);
		return_or_not = 1;
	}

	if (!return_or_not)
	{
		status = SREC_readAddress(line);

		if (Check_CheckSum(line) != SREC_CHECKSUM_OK)
		{
			status = Check_CheckSum(line);
			return_or_not = 1;
		}
	}

	if (!return_or_not && SREC_readData(line, size_field_data) != SREC_LINE_TRUE)
	{
		status = SREC_readData(line, size_field_data);
		return_or_not = 1;
	}

	if (!return_or_not)
	{
		status = SREC_LINE_TRUE;
	}

	return status;
}

static uint8_t convert_hex(char hex)
{
	uint8_t result;
	if (hex >= '0' && hex <= '9')
		result = hex - '0';
	else if (hex >= 'A' && hex <= 'F')
		result = hex - 'A' + 10;
	else if (hex >= 'a' && hex <= 'f')
		result = hex - 'a' + 10;
	else
		result = HEX_INVALID;
	return result;
}
static SREC_Status_Type str_to_hex(char *pStr, uint8_t len, uint32_t *pResult)
{
	uint8_t index;
	SREC_Status_Type status = SREC_CONVERT_STRING_OK;
	*pResult = 0;
	uint8_t tempHex;

	for (index = 0; index < len; index++)
	{
		tempHex = convert_hex(pStr[index]);
		if (HEX_INVALID == tempHex)
		{
			status = SREC_ERROR_CHARACTER;
			break;
		}
		*pResult += tempHex * pow(16, len - 1 - index);
	}

	return status;
}

static SREC_Status_Type Check_character(uint8_t *line_check, uint32_t line_number)
{
	uint32_t index = 0;
	SREC_Status_Type status = SREC_CHARACTER_OK;

	/* check file have empty line or not */
	if ((line_check[0] == '\r' && line_check[1] == '\n') || (line_check[0] == '\n'))
	{
		status = SREC_ERROR_EMPTY_LINE;
	}
	/* check on the line 1 ,type must be 0 */
	else if (line_number == 1 && line_check[1] != '0')
	{
		status = SREC_HEADER_ERROR;
	}
	/* check first character must be S */
	else if (line_check[0] != 'S')
	{
		status = SREC_ERROR_FIRST_CHARACTER;
	}
	else
	{
		/* check all character must be hex */
		for (index = 1; ((line_check[index] != '\n' && line_check[index] != '\r') &&
						 (line_check[1] != '9' && line_check[1] != '8' && line_check[1] != '7'));
			 index++)
		{
			if (convert_hex(line_check[index]) > 0x0F)
			{
				status = SREC_ERROR_CHARACTER;
			}
			if (line_check[index] == 'f' || line_check[index] == 'a' || line_check[index] == 'b' || line_check[index] == 'c' || line_check[index] == 'd' || line_check[index] == 'e')
			{
				status = SREC_ERROR_CHARACTER2;
			}
		}
	}
	return status;
}

static SREC_Status_Type Check_CheckSum(uint8_t *line)
{
	SREC_Status_Type status = SREC_CHECKSUM_OK;

	uint8_t *temp = &line[2];
	uint8_t data[60];
	uint8_t byteCount;
	char byte[2];
	uint8_t check_Checksum = 0;
	for (uint32_t i = 0; i < SrecData.ByteCount + 1; i++)
	{
		byte[0] = temp[i * 2];
		byte[1] = temp[i * 2 + 1];
		status = str_to_hex(byte, 2, (uint32_t *)&data[i]);
		if (i == SrecData.ByteCount)
		{
			status = str_to_hex(byte, 2, (uint32_t *)&SrecData.Checksum);
		}
		check_Checksum += data[i];
	}
	if (check_Checksum != 0xFF)
	{
		status = SREC_ERROR_CHECKSUM;
	}
	else
	{
		status = SREC_CHECKSUM_OK;
	}
	return status;
}

static SREC_Status_Type SREC_GetAndCheckByteCount(uint8_t *line)
{
	SREC_Status_Type status = SREC_BYTECOUNT_OK;
	uint8_t *temp = &line[2];

	uint8_t byteCount;
	char byte[2];
	byte[0] = line[2];
	byte[1] = line[3];
	byteCount = str_to_hex(byte, 2, ((uint32_t *)&SrecData.ByteCount));

	for (uint32_t i = 0; i < SrecData.ByteCount * 2 + 4; i++)
	{
		/* check case less than bytecount */
		if ((line[i] == '\r') || (line[i] == '\n'))
		{
			status = SREC_ERROR_LESS_BYTECOUNT;
		}

		/* check case have space in line */
		if (line[i] == ' ')
		{
			status = SREC_SPACE_IN_LINE;
		}
	}

	/* chekc case more byte than bytecount */
	if ((line[SrecData.ByteCount * 2 + 4] != '\n') && (line[SrecData.ByteCount * 2 + 4] != '\r') && (line[1] != '9' || line[1] != '7' || line[1] != '8') && (status == SREC_BYTECOUNT_OK))
	{
		status = SREC_ERROR_GREATER_BYTECOUNT;
	}
	return status;
}
static SREC_Status_Type SREC_CheckTypeSrec(uint8_t *line)
{
	SREC_Status_Type status = SREC_INVALID_TYPE;
	if ((line[1] == '1') || (line[1] == '2') || (line[1] == '3'))
	{
		status = SREC_TYPE_CONTAIN_DATA;
	}
	else if ((line[1] == '5') || (line[1] == '6'))
	{
		status = SREC_TYPE_COUNT_S5_S6;
	}
	else if ((line[1] == '7') || (line[1] == '8') || (line[1] == '9'))
	{
		status = SREC_TYPE_END_FILE_S9_S8_S7;
	}
	else if ((line[1] == '0'))
	{
		status = SREC_HEADER;
	}
	return status;
}

static SREC_Status_Type SREC_readAddress(uint8_t *line)
{
	SREC_Status_Type status = SREC_LINE_TRUE;
	uint8_t *temp = &line[4];
	AddressNum = (convert_hex(line[1]) + 1) * 2;
	char byte[convert_hex(AddressNum) + 1];
	for (uint32_t i = 0; i < convert_hex(AddressNum) + 1; i++)
	{
		byte[i] = temp[i];
	}
	status = str_to_hex(byte, AddressNum, (uint32_t *)&SrecData.Address);
	return status;
}

static SREC_Status_Type SREC_readData(uint8_t *line, uint8_t *size_field_data)
{
	SREC_Status_Type status = SREC_LINE_TRUE;
	uint8_t dataLen;
	uint8_t addrLen = (convert_hex(line[1]) + 1) * 2;
	uint8_t *temp = NULL;

	temp = &line[3 + 1 + addrLen];
	char byte[2];

	dataLen = SrecData.ByteCount - (addrLen / 2) - 1;
	*size_field_data = dataLen;
	SrecData.Data = (uint8_t *)malloc(sizeof(uint8_t) * dataLen);
	for (uint32_t i = 0; i < dataLen; i++)
	{
		byte[0] = temp[i * 2];
		byte[1] = temp[i * 2 + 1];
		status = str_to_hex(byte, 2, (uint32_t *)&SrecData.Data[i]);
	}
	return SREC_LINE_TRUE;
}
/******************************************************************************
 * End of filee
 ******************************************************************************/
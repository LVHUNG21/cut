	while (fgets(line, sizeof(line), file) != NULL && exit_loop == 1)
	{
		lineNum++;

		status = parse_line(lineNum, line, &size_field_data);
		// if ((status == SREC_ERROR_BYTECOUNT) || (status == SREC_ERROR_CHARACTER) ||
		// 	(status == SREC_ERROR_CHECKSUM) || (status == SREC_ERROR_EMPTY_LINE) ||
		// 	(status == SREC_ERROR_FIRST_CHARACTER) || (status == SREC_ERROR_GREATER_BYTECOUNT) ||
		// 	(status == SREC_ERROR_LESS_BYTECOUNT) || (status == SREC_HEADER_ERROR) ||
		// 	(status == SREC_ERROR_FORMAT) || (status == SREC_INVALID_TYPE) || (status == SREC_SPACE_IN_LINE))
		// {
		// 	flag_status = 'n';
		// }
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
	// if (flag_status == 'n')
	// {
	// 	status = SREC_FILE_ERROR;
	// }
	// else
	// {
	// 	status = SREC_PARSED;
	// }
	return status;
}
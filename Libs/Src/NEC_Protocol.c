#include "NEC_Protocol.h"

uint8_t NEC_Decode(uint32_t *raw_data_buf, uint8_t raw_data_len, uint8_t *address_result, uint8_t *command_result)
{
	uint32_t temp_data = 0;
	uint8_t index = 0;
	// Checking header mark
	if(!__IS_MATCH(raw_data_buf[index], NEC_HEADER_MARK)) return IR_DECODE_FAIL;
	index++;

	// Checking is it repeat signal
	if(	raw_data_len == 3 &&
		__IS_MATCH(raw_data_buf[index], NEC_REPEAT_SPACE) &&
		__IS_MATCH(raw_data_buf[index + 1], NEC_BIT_MARK))
	{
		return IR_DECODE_CPLT;
	}

	// Checking header space
	if(!__IS_MATCH(raw_data_buf[index], NEC_HEADER_SPACE)) return IR_DECODE_FAIL;
	index++;

	// Decode 32 bits data
	for(int i = 0; i < 32; i++)
	{
		if(!__IS_MATCH(raw_data_buf[index], NEC_BIT_MARK)) return IR_DECODE_FAIL;
		index++;

		if(!__IS_MATCH(raw_data_buf[index], NEC_BIT_ONE_SPACE))
		{
			temp_data = (temp_data << 1) | 1;
		}
		else if(!__IS_MATCH(raw_data_buf[index], NEC_BIT_ZERO_SPACE))
		{
			temp_data <<= 1;
		}
		else
		{
			return IR_DECODE_FAIL;
		}
		index++;
	}

	uint8_t temp_data_arr[4] = {(uint8_t)(temp_data >> 24), ~(uint8_t)(temp_data >> 16), (uint8_t)(temp_data >> 8), ~(uint8_t)(temp_data)};
	if((temp_data_arr[0] == temp_data_arr[1]) && (temp_data_arr[2] == temp_data_arr[3]))
	{
		*address_result = temp_data_arr[0];
		*command_result = temp_data_arr[2];
		return IR_DECODE_CPLT;
	}

	return IR_DECODE_FAIL;
}

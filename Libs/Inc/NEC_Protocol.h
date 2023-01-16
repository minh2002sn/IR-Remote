#ifndef NEC_PROTOCOL_H
#define NEC_PROTOCOL_H

#include <stdint.h>
#include "IR_Remote.h"

// Signal time in us
#define NEC_HEADER_MARK		9000
#define NEC_HEADER_SPACE	4500
#define NEC_BIT_MARK		640
#define NEC_BIT_ONE_SPACE	1600
#define NEC_BIT_ZERO_SPACE	560
#define NEC_REPEAT_SPACE	2250

// NEC decoding function
uint8_t NEC_Decode(uint32_t *raw_data_buf, uint8_t raw_data_len, uint8_t *address_result, uint8_t *command_result);

#endif

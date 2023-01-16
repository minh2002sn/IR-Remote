#ifndef IR_REMOTE_H
#define IR_REMOTE_H

#include <stdint.h>
#include "main.h"
#include "NEC_Protocol.h"

#define MAX_RAW_DATA_LEN	100
#define IR_TIMEOUT_VALUE	5000
#define ERROR_PERCENT		0.1

#define __IS_MATCH(measure_time, desired_time)		((measure_time >= desired_time * (1 - ERROR_PERCENT)) && (measure_time <= desired_time * (1 + ERROR_PERCENT)))

// Decoding type
// Add more if using more decoding types
typedef enum
{
	DECODE_TYPE_NEC,
} DECODING_TYPE_t;

// Decoded data structure
// Add more struct into union if using more decoding types
typedef struct
{
	DECODING_TYPE_t decoding_type;
	union
	{
		struct
		{
			uint8_t address;
			uint8_t command;
		} NEC;
	} value;
} DECODED_DATA_t;

// Decoding status
enum
{
	IR_DECODE_FAIL,
	IR_DECODE_CPLT,
};

// Logic level for mark and space signal
enum
{
	IR_MARK_LOGIC_LEVEL,
	IR_SPACE_LOGIC_LEVEL,
};

// IR receiving state
enum
{
	IR_IDLE_STATE,
	IR_RECEIVING_MARK_STATE,
	IR_RECEIVING_SPACE_STATE,
	IR_STOP_RECEIVNING_STATE,
};

typedef struct
{
	// Normal variables
	GPIO_TypeDef *rx_gpio_port;					// Port of rx pin
	uint16_t rx_gpio_pin;						// Rx pin
	TIM_HandleTypeDef *htim;					// Timer used for counting time

	// Receiver variable
	uint8_t receiving_state;					// Receiving state
	uint32_t timer;								// Store time value in us unit
	uint32_t raw_data_buf[MAX_RAW_DATA_LEN];	// Store raw value in us unit
	uint8_t raw_data_len;						// Number of vale in raw_data_buf
	DECODED_DATA_t decoded_data;
} IR_DATA_t;

// Initialization function
void IR_REMOTE_Init(IR_DATA_t *ir_data, GPIO_TypeDef *gpio_port, uint16_t gpio_pin, TIM_HandleTypeDef *htim, DECODING_TYPE_t decoding_type);

// Call this function in external interrupt function
void IR_REMOTE_EXTI_Handle(IR_DATA_t *ir_data);
// Call this function in timer update interrupt function
void IR_REMOTE_TIMI_Handle(IR_DATA_t *ir_data, TIM_HandleTypeDef *htim);

// These 2 functions are called when decoding process done
// MUST REDEFINITION these 2 functions to use
void IR_REMOTE_RxCpltCallback(DECODED_DATA_t *decode_value);
void IR_REMOTE_RxFailCallback();

#endif

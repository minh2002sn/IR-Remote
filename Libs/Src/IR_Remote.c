#include "IR_Remote.h"

__weak void IR_REMOTE_RxCpltCallback(DECODED_DATA_t *decode_value){}
__weak void IR_REMOTE_RxFailCallback(){}

/*
 * @brief	IR protocol decoding function
 * 			It calls the callback function for decoding success or fail case
 * @note	Add more case in switch-case structure if using more decoding types
 * @param	ir_data			Pointer to IR_DATA_t structure
 * @retval	None
 */
static void decode(IR_DATA_t *ir_data)
{
	switch(ir_data->decoded_data.decoding_type)
	{
		case DECODE_TYPE_NEC:
			if(NEC_Decode(	ir_data->raw_data_buf, ir_data->raw_data_len,
							&ir_data->decoded_data.value.NEC.address,
							&ir_data->decoded_data.value.NEC.command))
			{
				IR_REMOTE_RxCpltCallback(&ir_data->decoded_data);
			}
			else
			{
				IR_REMOTE_RxFailCallback();
			}
			break;
	}
}

/*
 * @brief	IR protocol initialization function
 * @note	Configure timer with 2MHz frequency
 * @param	ir_data			Pointer to IR_DATA_t structure
 * @param	gpio_port		Pointer to GPIO_TypeDef structure of IR receiver's signal pin
 * @param	gpio_pin		Pin of IR receiver's signal pin
 * @param	htim			Pointer to TIM_HandleTypeDef structure of timer 2MHz frequency
 * @param	decoding_type	Decoding type of IR receiver
 * 							This parameter can be one of the GPIO_PinState enum values
 * @retval	None
 */
void IR_REMOTE_Init(IR_DATA_t *ir_data, GPIO_TypeDef *gpio_port, uint16_t gpio_pin, TIM_HandleTypeDef *htim, DECODING_TYPE_t decoding_type)
{
	ir_data->rx_gpio_port = gpio_port;
	ir_data->rx_gpio_pin = gpio_pin;
	ir_data->htim = htim;
	ir_data->decoded_data.decoding_type = decoding_type;
}

/*
 * @brief	Detect first edge of signal
 * @note	Call this function in external interrupt function
 * @param	ir_data			Pointer to IR_DATA_t structure
 * @retval	None
 */
void IR_REMOTE_EXTI_Handle(IR_DATA_t *ir_data)
{
	// Start receiving process when EXTI pin detect 1st falling edge.
	if(!HAL_GPIO_ReadPin(ir_data->rx_gpio_port, ir_data->rx_gpio_pin) && ir_data->receiving_state == IR_IDLE_STATE)
	{
		// Set receiving state
		ir_data->receiving_state = IR_RECEIVING_MARK_STATE;
		// Reset data
		ir_data->timer = 0;
		ir_data->raw_data_len = 0;
		// Set counter register of timer to overflow after 50us
		ir_data->htim->Instance->CNT = -40;
		// Start timer
		HAL_TIM_Base_Start_IT(ir_data->htim);
	}
}

/*
 * @brief	Detect and store raw data of signal
 * @note	Call this function in timer update interrupt function
 * @param	ir_data			Pointer to IR_DATA_t structure
 * @retval	None
 */
void IR_REMOTE_TIMI_Handle(IR_DATA_t *ir_data, TIM_HandleTypeDef *htim)
{
	if(htim->Instance == ir_data->htim->Instance)
	{
		ir_data->htim->Instance->CNT = -40;
		//get rx pin's logic state in current time
		uint8_t rx_pin_state = HAL_GPIO_ReadPin(ir_data->rx_gpio_port, ir_data->rx_gpio_pin);

		ir_data->timer += 20; // + 50us every time timer overflow

		// Stop if buffer overflow
		if(ir_data->raw_data_len > MAX_RAW_DATA_LEN)
		{
			ir_data->receiving_state = IR_STOP_RECEIVNING_STATE;
		}

		switch(ir_data->receiving_state)
		{
			case IR_IDLE_STATE:
				break;
			case IR_RECEIVING_MARK_STATE:
				// Change state if rx pin change logic state
				if(rx_pin_state == IR_SPACE_LOGIC_LEVEL)
				{
					ir_data->raw_data_buf[ir_data->raw_data_len++] = ir_data->timer;
					ir_data->timer = 0;
					ir_data->receiving_state = IR_RECEIVING_SPACE_STATE;
				}
				break;
			case IR_RECEIVING_SPACE_STATE:
				// Change state if rx pin change logic state
				if(rx_pin_state == IR_MARK_LOGIC_LEVEL)
				{
					ir_data->raw_data_buf[ir_data->raw_data_len++] = ir_data->timer;
					ir_data->timer = 0;
					ir_data->receiving_state = IR_RECEIVING_MARK_STATE;
				}
				else
				{
					// Stop receiving process if it stay in space logic level 5ms
					if(ir_data->timer > IR_TIMEOUT_VALUE)
					{
						ir_data->receiving_state = IR_STOP_RECEIVNING_STATE;
					}
				}
				break;
			case IR_STOP_RECEIVNING_STATE:
				// Set receiving state to idle.
				ir_data->receiving_state = IR_IDLE_STATE;
				// Stop timer.
				HAL_TIM_Base_Stop_IT(ir_data->htim);
				decode(ir_data);
				return;
				break;
		}

	}
}


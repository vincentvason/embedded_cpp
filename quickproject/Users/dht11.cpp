#include "dht11.h"
#include <stdio.h>

DHT11::DHT11(GPIO_TypeDef *port, uint16_t pin, TIM_HandleTypeDef *htim) {
	this->port = port;
	this->pin = pin;
	this->htim = htim;
	setDHT11DataPinMode(GPIO_MODE_INPUT);
}

void DHT11::setDHT11DataPinMode(uint32_t p_mode) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	if (p_mode == GPIO_MODE_OUTPUT_PP) {
		GPIO_InitStruct.Pin = this->pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(this->port, &GPIO_InitStruct);
	} else if (p_mode == GPIO_MODE_INPUT) {
		GPIO_InitStruct.Pin = this->pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		HAL_GPIO_Init(this->port, &GPIO_InitStruct);
	}
}

void DHT11::delayMicroSeconds(uint32_t us) {
	__HAL_TIM_SET_COUNTER(htim, 0);
	while(__HAL_TIM_GET_COUNTER(htim) < us);
}

uint32_t DHT11::expectSignalMicroSeconds(uint8_t state) {
	__HAL_TIM_SET_COUNTER(htim, 0);
	while(HAL_GPIO_ReadPin(port, pin) == state)
	{
		if(__HAL_TIM_GET_COUNTER(htim) > 100)
		{
			break;
		}
	}
	return __HAL_TIM_GET_COUNTER(htim);
}

uint8_t cycle[80];
uint8_t DHT11::readDHT11() {
	uint8_t data[5];

	HAL_TIM_Base_Start(htim);
	setDHT11DataPinMode(GPIO_MODE_INPUT);
	HAL_Delay(1);
	setDHT11DataPinMode(GPIO_MODE_OUTPUT_PP);
	HAL_Delay(20);
	setDHT11DataPinMode(GPIO_MODE_INPUT);
	delayMicroSeconds(55);
	__disable_irq();

	if(expectSignalMicroSeconds(GPIO_PIN_RESET) > 100)
	{
		printf("DHT timeout waiting for start signal low pulse.\n");
		HAL_TIM_Base_Stop(htim);
		__enable_irq();
		return STATUS_ERROR;
	}
	if(expectSignalMicroSeconds(GPIO_PIN_SET) > 100)
	{
		printf("DHT timeout waiting for start signal high pulse.\n");
		HAL_TIM_Base_Stop(htim);
		__enable_irq();
		return STATUS_ERROR;
	}

	for(int i = 0; i < 80; i += 2)
	{
		cycle[i] = expectSignalMicroSeconds(GPIO_PIN_RESET);
		cycle[i + 1] = expectSignalMicroSeconds(GPIO_PIN_SET);
	}
	HAL_TIM_Base_Stop(htim);
	__enable_irq();

	for(int i = 0; i < 40; ++i)
	{
		uint32_t low_cycle = cycle[2 * i];
		uint32_t high_cycle = cycle[2 * i + 1];
		if((low_cycle > 500) || (high_cycle > 500))
		{
			printf("DHT timeout waiting for pulse.\n");
			return STATUS_ERROR;
		}
		data[i / 8] <<= 1;
		if (high_cycle > low_cycle) {
			// High cycles are greater than 50us low cycle count, must be a 1.
		    data[i / 8] |= 1;
		}
	}

	uint8_t chk = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
	printf("Received from DHT: %hu, %hu, %hu, %hu, %hu = %hu\n", data[0], data[1], data[2], data[3], data[4], chk);

	temperature = (float) data[2];
	if(data[3] & 0x80)
	{
		temperature = -1-temperature;
	}
	temperature += (data[3] & 0x0f) * 0.1F;

	humidity = (float) data[0] + data[1] * 0.1F;

	if(data[4] != chk)
	{
		return STATUS_CHECKSUM_FAIL;
	}
	else
	{
		return STATUS_OK;
	}


}

float DHT11::getTemperature() {
	return temperature;
}

float DHT11::getHumidity() {
	return humidity;
}

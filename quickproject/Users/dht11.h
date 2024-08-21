#ifndef DHT11_H_
#define DHT11_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"

class DHT11
{
//Variable
public:
	const uint8_t STATUS_OK = 0;
	const uint8_t STATUS_ERROR = 101;
	const uint8_t STATUS_CHECKSUM_FAIL = 102;

private:
	GPIO_TypeDef* port;
	uint16_t pin;
	TIM_HandleTypeDef *htim;
	float temperature;
	float humidity;

//Function
public:
	DHT11(GPIO_TypeDef* port, uint16_t pin, TIM_HandleTypeDef *htim);
	uint8_t readDHT11();
	float getTemperature();
	float getHumidity();

private:
	void setDHT11DataPinMode(uint32_t p_mode);
	void delayMicroSeconds(uint32_t us);
	uint32_t expectSignalMicroSeconds(uint8_t state);
};

#endif

/*
 * mpu6250.h
 *
 *  Created on: Aug 20, 2024
 *      Author: ASUS
 */

#ifndef MPU9250_H_
#define MPU9250_H_

#include "i2c.h"

typedef struct {
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
} float_mpu;

class MPU9250
{
//Enums
public:
	enum AD0{
		AD0_LOW = (uint8_t)0x68,
		AD0_HIGH = (uint8_t)0x69
	};

	enum AFSR{
		AFSR_2G,
		AFSR_4G,
		AFSR_8G,
		AFSR_16G,
		AFSR_DEFAULT
	};

	enum GFSR{
		GFSR_250DPS,
		GFSR_500DPS,
		GFSR_1000DPS,
		GFSR_2000DPS,
		GFSR_DEFAULT
	};

//Variables
private:
	I2C_HandleTypeDef* hi2c;
	uint8_t hi2c_addr;
	float a_scale_factor;
	float g_scale_factor;
	AFSR afsr = AFSR_4G;
	GFSR gfsr = GFSR_500DPS;

public:
	const uint8_t ERR = 0;
	const uint8_t OK = 1;

//Functions
public:
	MPU9250(I2C_HandleTypeDef* hi2c, AD0 hi2c_addr);
	uint8_t begin();
	void setAccelFSR(AFSR afsr = AFSR_DEFAULT);
	void setGyroFSR(GFSR gfsr = GFSR_DEFAULT);
	float_mpu readData();

};



#endif /* MPU9250_H_ */

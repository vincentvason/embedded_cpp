/*
 * mpu9250.cpp
 *
 *  Created on: Aug 20, 2024
 *      Author: ASUS
 */

#include "mpu9250.h"
#include <stdio.h>

#define ADDR_WHO_AM_I 		0x75
#define ADDR_AD0_LOW 		0x68
#define ADDR_ADO_HIGH		0x69
#define ADDR_GYRO_CONFIG	0x1B
#define ADDR_ACCEL_CONFIG 	0x1C
#define ADDR_PWR_MGMT_1		0x6B
#define ADDR_ACCEL_XOUT_H	0x3B

#define MPU9250_WHO_AM_I  	0x71
#define MPU6050_WHO_AM_I  	0x68
#define I2C_TIMEOUT_MS		1000

MPU9250::MPU9250(I2C_HandleTypeDef* hi2c, AD0 hi2c_addr)
{
	this->hi2c = hi2c;
	this->hi2c_addr = hi2c_addr;
}

uint8_t MPU9250::begin()
{
    // Initialize variables
    uint8_t check, select;

    // Confirm device
    HAL_I2C_Mem_Read(hi2c, hi2c_addr, ADDR_WHO_AM_I, 1, &check, 1, I2C_TIMEOUT_MS);
    printf("%hu", check);

    // TODO: If 9250 or 6050 fails could it trigger the opposite check???
    if ((check == MPU9250_WHO_AM_I) || (check == MPU6050_WHO_AM_I))
    {
        // Startup / reset the sensor
        select = 0x00;
        HAL_I2C_Mem_Write(hi2c, hi2c_addr, ADDR_PWR_MGMT_1, 1, &select, 1, I2C_TIMEOUT_MS);

        // Set the full scale ranges
        setAccelFSR();
        setGyroFSR();

        return OK;
    }
    else
    {
        return ERR;
    }
}

void MPU9250::setAccelFSR(AFSR afsr)
{
    // Variable init
    uint8_t select;
    this->afsr = afsr;

    // Set the value
    switch (afsr)
    {
    case AFSR_2G:
    	a_scale_factor = 16384.0;
        select = 0x00;
        break;
    case AFSR_4G:
    	a_scale_factor = 8192.0;
        select = 0x08;
        break;
    case AFSR_8G:
    	a_scale_factor = 4096.0;
        select = 0x10;
        break;
    case AFSR_16G:
    	a_scale_factor = 2048.0;
        select = 0x18;
        break;
    default:
    	setAccelFSR(this->afsr);
        return;
    }

    HAL_I2C_Mem_Write(hi2c, hi2c_addr, ADDR_ACCEL_CONFIG, 1, &select, 1, I2C_TIMEOUT_MS);
}

void MPU9250::setGyroFSR(GFSR gfsr)
{
    // Variable init
    uint8_t select;
    this->gfsr = gfsr;

    // Set the value
    switch (gfsr)
    {
    case GFSR_250DPS:
        g_scale_factor = 131.0;
        select = 0x00;
        break;
    case GFSR_500DPS:
    	g_scale_factor = 65.5;
        select = 0x08;
        break;
    case GFSR_1000DPS:
    	g_scale_factor = 32.0;
        select = 0x10;
        break;
    case GFSR_2000DPS:
    	g_scale_factor = 16.4;
        select = 0x18;
        break;
    default:
    	setGyroFSR(this->gfsr);
        return;
    }

    HAL_I2C_Mem_Write(hi2c, hi2c_addr, ADDR_GYRO_CONFIG, 1, &select, 1, I2C_TIMEOUT_MS);
}

float_mpu MPU9250::readData()
{
	uint8_t buf[14];
	float_mpu data;
	HAL_I2C_Mem_Read(hi2c, hi2c_addr, ADDR_ACCEL_XOUT_H, 1, buf, 14, I2C_TIMEOUT_MS);

	data.ax = (buf[0] << 8 | buf[1]) / a_scale_factor;
	data.ay = (buf[2] << 8 | buf[3]) / a_scale_factor;
	data.az = (buf[4] << 8 | buf[5]) / a_scale_factor;
	data.gx = (buf[8] << 8 | buf[9]) / g_scale_factor;
	data.gy = (buf[10] << 8 | buf[11]) / g_scale_factor;
	data.gz = (buf[12] << 8 | buf[13]) / g_scale_factor;

	return data;
}



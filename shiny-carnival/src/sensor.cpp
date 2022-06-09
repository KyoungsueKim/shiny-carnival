#include "sensor.h"
#include "socket.h"
#include "gpio.h"
#include "camera.h"
#include <cstdlib>
#include <cstdio>

int SensingWithEncoded(int mode)
{
	int pin1, pin2, pin3, shiftFactor1, shiftFactor2, shiftFactor3;

	// 어느쪽 라즈베리파이인지 모드 설정
	switch (mode)
	{
	case MODE_1:
		pin1 = SENSOR_1;
		pin2 = SENSOR_2;
		pin3 = SENSOR_3;
		shiftFactor1 = 2;
		shiftFactor2 = 4;
		shiftFactor3 = 8;
		break;

	case MODE_2:
		pin1 = SENSOR_4;
		pin2 = SENSOR_5;
		pin3 = SENSOR_6;
		shiftFactor1 = 16;
		shiftFactor2 = 32;
		shiftFactor3 = 64;
		break;

	default:
		printf("Invalid Sensing Mode!");
		return -1;
	}

	// 센서 GPIO Export
	if (GPIOExport(pin1) == -1 || GPIOExport(pin2) == -1 || GPIOExport(pin3))
	{
		return -1;
	}

	// 센서 GPIO의 Direction 설정
	if (GPIODirection(pin1, IN) == -1 || GPIODirection(pin2, IN) == -1 || GPIODirection(pin3, IN) == -1)
	{
		return -1;
	}

	// 센싱 후 데이터 인코딩
	int encodedData = (GPIORead(pin1) * shiftFactor1) + (GPIORead(pin2) * shiftFactor2) + (GPIORead(pin3) * shiftFactor3);

	return encodedData;
}

Directions DecodeSensorData(const char *encodedData1, const char *encodedData2)
{
	Directions decodedData = {.right = false, .front = false, .left = false};

	// 전처리
	int sensorData1 = (int)strtol(encodedData1, nullptr, 10);
	int sensorData2 = (int)strtol(encodedData2, nullptr, 10);
	if (sensorData1 == -1 || sensorData2 == -1)
	{
		printf("Decoding Sensor Data Occured!");
		return decodedData;
	}

	// 왼쪽 접근일 경우
	if (sensorData1 & (0b000001) || sensorData1 & (0b000010))
	{
		decodedData.left = true;
	}

	// 앞쪽 접근일 경우
	if (sensorData1 & (0b000100) || sensorData2 & (0b001000))
	{
		decodedData.front = true;
	}

	// 오른쪽 접근일 경우
	if (sensorData2 & (0b010000) || sensorData2 & (0b100000))
	{
		decodedData.right = true;
	}

	return decodedData;
}
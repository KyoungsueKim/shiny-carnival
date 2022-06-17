#include "client.h"
#include "socket.h"
#include "gpio.h"
#include "sensor.h"
#include <stdio.h>

/***
 * 클라이언트 코드를 시작합니다. 라즈베리파이에 따라 모드를 설정해야합니다.
 * @param mode : 클라이언트 라즈베리파이에 따라 MODE_1 또는 MODE_2
 */

int g_wave_distance_1 = 0;
int g_wave_distance_2 = 0;

void *wave_thd_1()
{
	clock_t start_t, end_t;
	double time;
	char msg[5];
	int str_len;

	if (-1 == GPIOExport(23) || -1 == GPIOExport(24))
		exit(0);
	usleep(100000);

	if (-1 == GPIODirection(23, OUT) || -1 == GPIODirection(24, IN))
		exit(0);

	GPIOWrite(23, 0);
	usleep(100000);
	while (1)
	{
		if (-1 == GPIOWrite(23, 1))
		{
			printf("Error\n");
			exit(0);
		}
		usleep(10);
		GPIOWrite(23, 0);
		while (GPIORead(24) == 0)
			start_t = clock();
		while (GPIORead(24) == 1)
			end_t = clock();
		time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
		distance = (time / 2) * 34000;
		if (distance > 500)
			distance = 500;
		g_wave_distance = distance;
		usleep(100000);
	}
}

void *wave_thd_2()
{
	clock_t start_t, end_t;
	double time;
	char msg[5];
	int str_len;

	if (-1 == GPIOExport(27) || -1 == GPIOExport(22))
		exit(0);
	usleep(100000);

	if (-1 == GPIODirection(27, OUT) || -1 == GPIODirection(22, IN))
		exit(0);

	GPIOWrite(27, 0);
	usleep(100000);
	while (1)
	{
		if (-1 == GPIOWrite(27, 1))
		{
			printf("Error\n");
			exit(0);
		}
		usleep(10);
		GPIOWrite(27, 0);
		while (GPIORead(22) == 0)
			start_t = clock();
		while (GPIORead(24) == 1)
			end_t = clock();
		time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
		distance = (time / 2) * 34000;
		if (distance > 500)
			distance = 500;
		g_wave_distance = distance;
		usleep(100000);
	}
}

void StartClient(int mode)
{
	pthread_t wave_1, wave_2;
	int tmp;
	ClientSocket clientSocket = ClientSocket();

	tmp = pthread_create(&wave_1, NULL, wave_thd_1, NULL);
	if (tmp < 0) {
		perror("thread create error : ");
		exit(0);
	}
	tmp = pthread_create(&wave_2, NULL, wave_thd, NULL);
	if (tmp < 0) {
		perror("thread create error : ");
		exit(0);
	}
	while (!clientInterrupt)
	{
		char sensingData[5];
		int data = SensingWithEncoded(mode);
		sprintf(sensingData, "%d", data);
		clientSocket.sendDataToServer(sensingData, sizeof(sensingData));
	}
}
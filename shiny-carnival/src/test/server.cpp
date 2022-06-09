#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include "piezo.hpp"
#include "pwm.hpp"

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define POUT 23
#define DIRECTION_MAX 45
#define PIN 24

#define VALUE_MAX 256
#define DIRECTION_MAX 45

int g_client1_data = 0;
int g_client2_data = 0;

typedef struct client_s
{
	int fd;
	char ip[30];
} client_t;

using namespace cv;
using namespace std;

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
static int GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open export for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

static int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return (-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[] = "in\0out";

	// char path[DIRECTION_MAX]="/sys/class/gpio/gpio24/direction";
	char path[DIRECTION_MAX] = "/sys/class/gpio/gpio%d/direction";
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);

	fd = open(path, O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return (-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3))
	{
		fprintf(stderr, "Failed to set direction!\n");
		return (-1);
	}

	close(fd);
	return (0);
}

static int GPIORead(int pin)
{
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return (-1);
	}

	if (-1 == read(fd, value_str, 3))
	{
		fprintf(stderr, "Failed to read value!\n");
		return (-1);
	}

	close(fd);

	return (atoi(value_str));
}

static int GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd)
	{
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return (-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1))
	{
		fprintf(stderr, "Failed to write value!\n");
		return (-1);

		close(fd);
		return (0);
	}
}

void *client_thread_create(void *data)
{
	int read_size;
	int msg_data = -1;
	int left_or_right = -1;
	char msg[5];
	client_t *client;

	client = (client_t *)data;
	printf("client ip = %s\n", client->ip);
	// 왼쪽
	if (strcmp(client->ip, "192.168.1.5") == 0)
		left_or_right = 0;
	// 오른쪽
	else if (strcmp(client->ip, "192.168.1.4") == 0)
		left_or_right = 1;
	while (1)
	{
		read_size = read(client->fd, msg, sizeof(msg));
		msg_data = atoi(msg);
		if (read_size <= 0)
			break;
		else
		{
			// left raspberry pi
			if (left_or_right == 0)
				g_client1_data = msg_data;
			// right raspberry pi
			else if (left_or_right == 1)
				g_client2_data = msg_data;
		}
	}
	close(client->fd);
}

void *camera_control(void *data)
{
	VideoCapture cap(-1);

	if (!cap.isOpened())
	{
		printf("camera open error\n");
		exit(1);
	}
	Mat img;
	while (1)
	{
		cap >> img;
		transpose(img, img);
		flip(img, img, 1);
		transpose(img, img);
		flip(img, img, 1);

		if (g_client1_data >= 0 && g_client2_data >= 0)
		{
			if (g_client1_data & (1 << 1) || g_client1_data & (1 << 2))
			{
				// 왼쪽 가이드 라인 생성
				ellipse(img, Point(0, 240), Size(220.0, 70.0), 90, 0, 360, Scalar(0, 0, 255), 5, 8);
			}
			if (g_client1_data & (1 << 3) || g_client2_data & (1 << 4))
			{
				// 앞쪽 가이드 라인 생성
				ellipse(img, Point(320, 0), Size(300.0, 70.0), 180, 0, 360, Scalar(0, 0, 255), 5, 8);
			}
			if (g_client2_data & (1 << 5) || g_client2_data & (1 << 6))
			{
				// 오른쪽 가이드 라인 생성
				ellipse(img, Point(640, 240), Size(220.0, 70.0), 90, 0, 360, Scalar(0, 0, 255), 5, 8);
			}
		}
		imshow("camera", img);
		if (waitKey(1) == 27)
			break;
	}
}

void *piezo_control(void *data)
{
	Melody warning = Melody();
	warning.melody = {E, Dsharp, E, Dsharp, E, lowB, D, C,
					  lowA, ZERO, ZERO, lowC, lowE, lowA, lowB, ZERO, lowE,
					  lowG, lowB, C, ZERO, ZERO, E, Dsharp, E, Dsharp, E,
					  lowB, D, C, lowA, ZERO, ZERO, lowC, lowE, lowA, lowB,
					  ZERO, lowE, C, lowB, lowA, ZERO, ZERO};
	while (1)
	{
		if (g_client1_data > 0 || g_client2_data > 0)
		{
			warning.playMelody(0.3);
		}
		else
			PWMWriteDutyCycle(0, 0);
	}
}

int main(void)
{
	int serv_sock, clnt_sock_fd1, clnt_sock_fd2;
	struct sockaddr_in serv_addr, clnt1_addr, clnt2_addr;
	socklen_t clnt1_addr_size, clnt2_addr_size;
	client_t client_data[2];
	pthread_t client_thread[2];
	pthread_t camera_control_thread, piezo_control_thread;

	// socket연결
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8080);
	if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt1_addr_size = sizeof(clnt1_addr);
	clnt2_addr_size = sizeof(clnt2_addr);

	clnt_sock_fd1 = accept(serv_sock, (struct sockaddr *)&clnt1_addr, &clnt1_addr_size);
	if (clnt_sock_fd1 == -1)
		error_handling("accept() error");
	clnt_sock_fd2 = accept(serv_sock, (struct sockaddr *)&clnt2_addr, &clnt2_addr_size);
	if (clnt_sock_fd2 == -1)
		error_handling("accept() error");

	client_data[0].fd = clnt_sock_fd1;
	client_data[1].fd = clnt_sock_fd2;

	strcpy(client_data[0].ip, inet_ntoa(clnt1_addr.sin_addr));
	strcpy(client_data[1].ip, inet_ntoa(clnt2_addr.sin_addr));

	pthread_create(&client_thread[0], NULL, client_thread_create, &client_data[0]);
	pthread_create(&client_thread[1], NULL, client_thread_create, &client_data[1]);
	pthread_create(&camera_control_thread, NULL, camera_control, NULL);
	pthread_create(&piezo_control_thread, NULL, NULL);

	pthread_join(client_thread[0], NULL);
	pthread_join(client_thread[1], NULL);
	pthread_join(camera_control_thread, NULL);
	pthread_join(piezo_control_thread, NULL);

	close(clnt_sock_fd1);
	close(clnt_sock_fd2);
	close(serv_sock);
	// Disable GPIO pins
	if (-1 == GPIOUnexport(PIN) || -1 == GPIOUnexport(POUT))
		return (4);
	return (0);
}

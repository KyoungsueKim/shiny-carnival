#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define POUT 23
#define PIN 24
#define SENSOR 25
#define VALUE_MAX 45
#define DIRECTION_MAX 45
#define DISTANCE 10

static int PWMExport (int pwmnum) {
    #define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    int bytes_written;
    int fd;

    fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open in unexport!\n");
        return (-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pwmnum);
    write(fd, buffer, bytes_written);
    close(fd);

    sleep(1);
    fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open in export!\n");
        return (-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pwmnum);
    write(fd, buffer, bytes_written);
    close(fd);
    sleep(1);
    return (0);
}

static int PWMEnable (int pwmnum) {
    static const char s_unenable_str[] = "0";
    static const char s_enable_str[] = "1";
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/pwm/pwmchip0/pwm%d/enable", pwmnum);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open in enable! \n");
        return -1;
    }

    write(fd, s_unenable_str, strlen(s_unenable_str));
    close(fd);

    fd = open(path, O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open in enable! \n");
        return -1;
    }

    write(fd, s_enable_str, strlen(s_enable_str));
    close(fd);
    return (0);
}

static int PWMWritePeriod(int pwmnum, int value){
    char s_values_str[VALUE_MAX];
    char path[VALUE_MAX];
    int fd, byte;

    snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm%d/period", pwmnum);
    fd = open(path, O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open in period!\n");
        return (-1);
    }

    byte = snprintf(s_values_str, VALUE_MAX, "%d", value);
    if (-1 == write(fd, s_values_str, byte)) {
        fprintf(stderr, "Failed to write value in period!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return(0);
}

static int PWMWriteDutyCycle(int pwmnum, int value){
    char path[VALUE_MAX];
    char s_values_str[VALUE_MAX];
    int fd, byte;

    snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", pwmnum);
    fd = open(path, O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open in duty_cycle!\n");
        return -1;
    }

    byte = snprintf(s_values_str, VALUE_MAX, "%d", value);
    if (-1 == write(fd, s_values_str, byte)) {
        fprintf(stderr, "Failed to write value! in duty_cycle\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); fputc('\n',stderr);
	exit(1);
}
static int GPIOExport(int pin) {
#define BUFFER_MAX 3
   char buffer[BUFFER_MAX];
   ssize_t bytes_written;
   int fd;

   fd = open("/sys/class/gpio/export", O_WRONLY);
   if (-1 == fd) {
	  fprintf(stderr, "Failed to open export for writing!\n");
	  return(-1);
   }

   bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
   write(fd, buffer, bytes_written);
   close(fd);
   return(0);
}

static int GPIOUnexport(int pin) {
   char buffer[BUFFER_MAX];
   ssize_t bytes_written;
   int fd;

   fd = open("/sys/class/gpio/unexport", O_WRONLY);
   if (-1 == fd) {
	  fprintf(stderr, "Failed to open unexport for writing!\n");
	  return(-1);
   }

   bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
   write(fd, buffer, bytes_written);
   close(fd);
   return(0);
}

static int GPIODirection(int pin, int dir) {
   static const char s_directions_str[]  = "in\0out";

   char path[DIRECTION_MAX]="/sys/class/gpio/gpio%d/direction";
   int fd;

   snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);

   fd = open(path, O_WRONLY);
   if (-1 == fd) {
	  fprintf(stderr, "Failed to open gpio direction for writing!\n");
	  return(-1);
   }

   if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
	  fprintf(stderr, "Failed to set direction!\n");
	  return(-1);
   }

   close(fd);
   return(0);
}

static int GPIORead(int pin) {
   char path[VALUE_MAX];
   char value_str[4];
   int fd;

   snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
   fd = open(path, O_RDONLY);
   if (-1 == fd) {
	  fprintf(stderr, "Failed to open gpio value for reading!\n");
	  return(-1);
   }

   if (-1 == read(fd, value_str, 3)) {
	  fprintf(stderr, "Failed to read value!\n");
	  return(-1);
   }

   close(fd);

   return(atoi(value_str));
}

static int GPIOWrite(int pin, int value) {
   static const char s_values_str[] = "01";

   char path[VALUE_MAX];
   int fd;

   snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
   fd = open(path, O_WRONLY);
   if (-1 == fd) {
	  fprintf(stderr, "Failed to open gpio value for writing!\n");
	  return(-1);
   }

   if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
	  fprintf(stderr, "Failed to write value!\n");
	  return(-1);

   close(fd);
   return(0);
   }
}

int main(void)
{
    int msg_data;
	struct sockaddr_in serv_addr;
	char msg[5];

    if(-1 == GPIOExport(6) || -1 == GPIOExport(23) || -1 == GPIOExport(25)) {
        printf("GPIO Export error\n");
        return 1;
    }
    
    if(-1 == GPIODirection(6, IN) || -1 == GPIODirection(23, IN) || -1 == GPIODirection(25, IN)) {
        printf("dir error\n");
        return 1;
    }

	int sock = socket(PF_INET, SOCK_STREAM, 0); if(sock == -1)
	error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
	serv_addr.sin_port = htons(atoi("8080"));
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	while (1)
    {
        int sensor4 = GPIORead(25) * 16;
        int sensor5 = GPIORead(23) * 32;
        int sensor6 = GPIORead(6) * 64;  
        int sum = sensor4 + sensor5 + sensor6;
        sprintf(msg, "%d", sum);
        // printf("4 : %d, 5 : %d, 6: %d\n", sensor4, sensor5, sensor6);
        write(sock, msg, sizeof(msg));
        usleep(100000);
    }

	close(sock);
	//Disable GPIO pins
	if (-1 == GPIOUnexport(POUT) || -1 == GPIOUnexport(PIN))
		return(4);
	return(0);
}
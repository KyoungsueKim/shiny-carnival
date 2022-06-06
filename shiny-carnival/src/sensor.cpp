#include "sensor.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//#include <pthread.h>
#include <time.h>

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define VALUE_MAX 45
#define DIRECTION_MAX 45
#define DISTANCE 10
#define BUFFER_MAX 3


void sensor(){
    #define LED1 22
    #define SENSOR1 23
    #define LED2 24
    #define SENSOR2 25
    #define LED3 5
    #define SENSOR3 6

    if(-1 == GPIOExport(LED1))
    {
        printf("LED1 Export error\n");
        return;
    }

    if(-1 == GPIOExport(LED2))
    {
        printf("LED2 Export error\n");
        return;
    }

    if(-1 == GPIOExport(LED3))
    {
        printf("LED3 Export error\n");
        return;
    }

    if(-1 == GPIOExport(SENSOR1))
    {
        printf("SENSOR1 Export error\n");
        return;
    }

    if(-1 == GPIOExport(SENSOR2))
    {
        printf("SENSOR2 Export error\n");
        return;
    }
    
    if(-1 == GPIOExport(SENSOR3))
    {
        printf("SENSOR3 Export error\n");
        return;
    }

    if(-1 == GPIODirection(LED1, OUT))
    {
        printf("LED1 dir error\n");
        return;
    }

    if(-1 == GPIODirection(SENSOR1, IN))
    {
        printf("SENSOR1 dir error\n");
        return;
    }

    if(-1 == GPIODirection(LED2, OUT))
    {
        printf("LED2 dir error\n");
        return;
    }

    if(-1 == GPIODirection(SENSOR2, IN))
    {
        printf("SENSOR2 dir error\n");
        return;
    }

    if(-1 == GPIODirection(LED3, OUT))
    {
        printf("LED3 dir error\n");
        return;
    }

    if(-1 == GPIODirection(SENSOR3, IN))
    {
        printf("SENSOR3 dir error\n");
        return;
    }

    
    GPIOWrite(LED2, LOW);
    int i = 0;
    int test = 100000;

    while(test--)
    {
        int isDetected = GPIORead(SENSOR2);
        if(!isDetected)
        {
            GPIOWrite(LED2, LOW);
            printf("LED2 OFF : %d\n",i);
            i++;
        }
        else
        {
            GPIOWrite(LED2, HIGH);
            printf("LED2 ON : %d\n",i);
            i++;
        }
        usleep(50);
    }
    
    if(-1 == GPIOUnexport(SENSOR2))
        return;
    if(-1 == GPIOUnexport(LED2))
        return;
}
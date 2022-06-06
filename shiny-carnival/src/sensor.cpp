#include "gpio.h"
#include "sensor.h"
#include <stdio.h>
#include <unistd.h>
#include "socket.h"

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define VALUE_MAX 45
#define DIRECTION_MAX 45
#define DISTANCE 10
#define BUFFER_MAX 3

void sensor(){
    #define LED1 5
    #define SENSOR1 6
    #define LED2 22
    #define SENSOR2 23
    #define LED3 24
    #define SENSOR3 25


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

    GPIOWrite(LED1, LOW);
    GPIOWrite(LED2, LOW);
    GPIOWrite(LED3, LOW);

    int i = 0;
    int test = 10000000;

    while(test--)
    {
        int isDetected1 = GPIORead(SENSOR1);
        //int isDetected2 = GPIORead(SENSOR2);
       // int isDetected3 = GPIORead(SENSOR3);

        if(isDetected1)
        {
            GPIOWrite(LED1, HIGH);
            printf("LED1 ON : %d\n",i);
            i++;
        }
        else{
            GPIOWrite(LED1, LOW);
            printf("LED off : %d\n",i);
            i++;
        }
        usleep(10000);

        // if(isDetected2)
        // {

        // }
        // if(isDetected3)
        // {

        // }

        // if(!isDetected1)
        // {
        //     GPIOWrite(LED2, LOW);
        //     printf("LED2 OFF : %d\n",i);
        //     i++;
        // }
        // else
        // {
        //     GPIOWrite(LED2, HIGH);
        //     printf("LED2 ON : %d\n",i);
        //     i++;
        // }
        // usleep(50);

    }
    
    if(-1 == GPIOUnexport(SENSOR2))
        return;
    if(-1 == GPIOUnexport(LED2))
        return;
}
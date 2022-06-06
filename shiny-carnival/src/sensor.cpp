#include "sensor.h"
#include <stdio.h>
#include <unistd.h>

// #include <stdlib.h>
// #include <time.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <fcntl.h>
// #include <string.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <pthread.h>


#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define POUT 23
#define PIN 24
#define VALUE_MAX 45
#define DIRECTION_MAX 45
#define DISTANCE 10
#define BUFFER_MAX 3

void sensor() {
    #define LED 24
    #define SENSOR 25

   if(-1 == GPIOExport(LED))
        return;

   if(-1 == GPIOExport(SENSOR))
        return;

   if(-1 == GPIODirection(LED, OUT))
   {
        printf("LED dir error\n");
        return;
   }

    if(-1 == GPIODirection(SENSOR, IN))
    {
        printf("SENSOR dir error\n");
        return;
    }

    GPIOWrite(LED, LOW);
    int i = 0;
    int test = 100000;

    while(test--)
    {
        int isDetected = GPIORead(SENSOR);
        if(!isDetected)
        {
            GPIOWrite(LED, LOW);
            printf("LED OFF : %d\n",i);
            i++;
        }
        else
        {
            GPIOWrite(LED, HIGH);
            printf("LED ON : %d\n",i);
            i++;
        }
        usleep(50);
    }
    
    if(-1 == GPIOUnexport(SENSOR))
        return;
    if(-1 == GPIOUnexport(LED))
        return;
}
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>
#include "camera.h"
#include "piezo.h"
#include "sensor.h"
#include "socket.h"

/***
 * 라즈베리파이의 Device UUID를 반환합니다. 
 * @return : 현재 실행되고있는 라즈베리파이의 Device UUID
 */
static char* getUUID(){
    // length of raspberry device uuid.
    char *uuid = (char*)malloc(32 * sizeof(char)); 
    
    FILE* uuid_file = fopen("/etc/machine-id", "r");
    fgets(uuid, 32, uuid_file);

    // printf("Device ID : %s\n", uuid);

    fclose(uuid_file);

    return uuid;
}

int main(void){

    // 1번 라즈베리파이
    if (strcmp(getUUID(), "401d9479da094aa6a86b4e18d9bf676") == 0){
        printf("Raspberry Pi 1 Initialized!\n");
    }


    // 2번 라즈베리파이
    if (strcmp(getUUID(), "316ae05cfd87495fa67a75f8ec53365") == 0){
        printf("Raspberry Pi 2 Initialized!\n");
    }

    return 0;
}
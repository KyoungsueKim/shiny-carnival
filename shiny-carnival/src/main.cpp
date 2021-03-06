#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camera.h"
#include "piezo.h"
#include "sensor.h"
#include "socket.h"
#include "server.h"
#include "client.h"

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
        StartServerThread();
    }

    // 2번 라즈베리파이
    if (strcmp(getUUID(), "316ae05cfd87495fa67a75f8ec53365") == 0){
        printf("Raspberry Pi 2 Initialized!\n");
        StartClient(MODE_1);
    }

    // 3번 라즈베리파이
    if (strcmp(getUUID(), "9713a22a4fcc4ae58e1fe2f9f301c82") == 0){
        printf("Raspberry Pi 3 Initialized!\n");
        StartClient(MODE_2);
    }

    return 0;
}
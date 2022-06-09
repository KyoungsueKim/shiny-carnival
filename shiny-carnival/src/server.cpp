#include "server.h"
#include "socket.h"
#include <pthread.h>
#include <stdio.h>
#include "camera.h"
#include "sensor.h"
#include "piezo.h"

typedef struct melodyThreadArguments {
    Melody melody;
    bool play;
} melodyThreadArguments;

void* ServerThread(void* args);
void StartServerThread(){
    pthread_t serverThread;
    pthread_create(&serverThread, NULL, ServerThread, NULL);
    pthread_join(serverThread, NULL);
}

void* playPiezoThread(void* args){
    melodyThreadArguments* arguments = (melodyThreadArguments*) args;
    Melody melody = arguments->melody;
    bool play = arguments->play;

    if(play){
        melody.playMelody(0.3);
    } else {
        melody.stopMelody();
    }
}

void* ServerThread(void* args){
    Camera camera = Camera();
    Melody warningSound = Melody();

    // melody 플레이 스레딩을 위한 전처리
    pthread_t melodyThread;
    struct melodyThreadArguments melodyArguments = {.melody = warningSound, .play = false};
    pthread_create(&melodyThread, NULL, playPiezoThread, &melodyArguments);

    ServerSocket serverSocket = ServerSocket();
    for(int i = 0; i < sizeof(serverSocket.clientData) / sizeof(serverSocket.clientData[0]); i++){
        printf("Server: %s connected", serverSocket.clientData[i].ip);
    }

    while (true) {
        //Read Data from StartClient
        const char* read[CLIENT_NUMBERS];
        for(int i = 0; i < CLIENT_NUMBERS; i++){
            read[i] = serverSocket.readDataFromClient(serverSocket.clientData[i]);
        }

        // Decoding directions
        Directions directions = DecodeSensorData(
                serverSocket.readDataFromClient(serverSocket.clientData[0]),
                serverSocket.readDataFromClient(serverSocket.clientData[1])
                );

        // Set guides by readDataFromClient
        int result = camera.SetGuides(directions);

        // Play Warning Sound using threading
        melodyArguments.play = result ? true : false;
        pthread_join(melodyThread, NULL);
    }
}
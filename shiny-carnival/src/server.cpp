#include "server.h"
#include "socket.h"
#include <pthread.h>
#include <stdio.h>
#include "camera.h"
#include "sensor.h"
#include "piezo.h"

typedef struct melodyThreadArguments
{
	Melody melody;
	bool play;
} melodyThreadArguments;

typedef struct displayCameraThreadArguments
{
	Camera* camera;
} displayCameraThreadArguments;

void *ServerThread(void *args);
void StartServerThread()
{
	pthread_t serverThread;
	pthread_create(&serverThread, NULL, ServerThread, NULL);
	pthread_join(serverThread, NULL);
}

void *DisplayCameraThread(void *args){
	displayCameraThreadArguments *arguments = (displayCameraThreadArguments *)args;
	Camera *camera = arguments->camera;

	while(1){
		camera->DisplayCamera();
	}
}

void *playPiezoThread(void *args)
{
	melodyThreadArguments *arguments = (melodyThreadArguments *)args;
	Melody melody = arguments->melody;
	bool play = arguments->play;

	if (play)
	{
		melody.playMelody(0.3);
	}
	else
	{
		melody.stopMelody();
	}
}

void *ServerThread(void *args)
{
	Camera camera = Camera();
	Melody warningSound = Melody();

	// melody 플레이 스레딩을 위한 전처리
	pthread_t melodyThread;
	melodyThreadArguments melodyArguments = {.melody = warningSound, .play = false};
	pthread_create(&melodyThread, NULL, playPiezoThread, &melodyArguments);

	// display Camera 스레딩을 위한 전처리
	pthread_t displayCameraThread;
	displayCameraThreadArguments displayCameraArguments = {.camera = &camera};
	pthread_create(&displayCameraThread, NULL, DisplayCameraThread, &displayCameraArguments);

	ServerSocket serverSocket = ServerSocket();
	for (int i = 0; i < sizeof(serverSocket.clientData) / sizeof(serverSocket.clientData[0]); i++)
	{
		printf("Server: %s connected\n", serverSocket.clientData[i].ip);
	}

	while (true)
	{
		// Read Data from StartClient
		char read[CLIENT_NUMBERS][5];
		for (int i = 0; i < CLIENT_NUMBERS; i++)
		{
			int data = serverSocket.readDataFromClient(serverSocket.clientData[i]);
			sprintf(read[i], "%d", data);
		}

		// Decoding directions
		Directions directions = DecodeSensorData(
			read[0], read[1]);

		pthread_detach(displayCameraThread);
		// Set guides by readDataFromClient
		int result = camera.SetGuides(directions);

		// Play Warning Sound using threading
		melodyArguments.play = result ? true : false;
		pthread_detach(melodyThread);
	}
}
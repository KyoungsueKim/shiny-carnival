#include "client.h"
#include "socket.h"
#include "sensor.h"
#include <stdio.h>

/***
 * 클라이언트 코드를 시작합니다. 라즈베리파이에 따라 모드를 설정해야합니다.
 * @param mode : 클라이언트 라즈베리파이에 따라 MODE_1 또는 MODE_2
 */
void StartClient(int mode)
{
	ClientSocket clientSocket = ClientSocket();
	while (!clientInterrupt)
	{
		char sensingData[5];
		int data = SensingWithEncoded(mode);
		sprintf(sensingData, "%d", data);
		clientSocket.sendDataToServer(sensingData, sizeof(sensingData));
	}
}
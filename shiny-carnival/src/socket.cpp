#include "socket.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

ServerSocket::ServerSocket() {
    // socket 개방
    serverFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (serverFileDescriptor == -1){
        error_handling("Server Socket Open Error");
    }

    // 변수 초기화
    memset(&serverAddress, 0, sizeof(serverAddress)); 

    // 서버 변수들 bind
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(8080);
    if(bind(serverFileDescriptor, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1){
        error_handling("Server Bind Error");
    }
	
    // Listening
    if(listen(serverFileDescriptor, 5) == -1) {
        error_handling("Server Listen Error");
    }

    // Accept from client request
    // socklen_t clientAddressSize = sizeof(clientData[0].fd) / CLIENT_NUMBERS;
    socklen_t clientAddressSize = sizeof(clientData[0]);
    for (int i = 0; i < CLIENT_NUMBERS; i++) {
        // Accepting (Getting client file descriptor)
        clientData[i].fd = accept(serverFileDescriptor, (struct sockaddr *)&clientAddress[i], &clientAddressSize);
        if (clientData[i].fd == -1){
            error_handling("Server Accept Error");
            return;
        }

        // Getting client ip
        strcpy(clientData[i].ip, inet_ntoa(clientAddress[i].sin_addr));
    }
}

ServerSocket::~ServerSocket(){
    // 클라이언트 소켓 클로즈
    for (int i = 0; i < sizeof(clientData); i++){
        close(this->clientData->fd);
    }

    // 서버 소켓 클로즈
    close(serverFileDescriptor);
}

const char* ServerSocket::readDataFromClient(client client){
    printf("Client IP: %s Connected\n", client.ip);

    char buffer[5];
    read(client.fd, buffer, sizeof(buffer));

    // 읽은 메시지가 하나라도 있다면 그대로 리턴. 아니라면 nullptr 리턴
    return atoi(buffer) > 0 ? buffer : nullptr;
}


ClientSocket::ClientSocket(){
    // Getting Socket Descriptor 
    clientFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (clientFileDescriptor == -1){
        error_handling("Client Socket Error");
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.2");
    serverAddress.sin_port = htons(atoi("8080"));

    if (connect(clientFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
        error_handling("Connect Error");
    }
}

ClientSocket::~ClientSocket(){
    close(clientFileDescriptor);
}

int ClientSocket::sendDataToServer(const char* message){
    return write(clientFileDescriptor, message, sizeof(message));
}
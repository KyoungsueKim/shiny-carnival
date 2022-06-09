#include <arpa/inet.h>

#define CLIENT_NUMBERS 2

typedef struct client
{
	int fd;
	char ip[30];
} client;

class ServerSocket
{
private:
	int serverFileDescriptor;
	int receivedData;

	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress[CLIENT_NUMBERS];

public:
	client clientData[CLIENT_NUMBERS];

	ServerSocket();
	~ServerSocket();
	int readDataFromClient(client client);
};

class ClientSocket
{
private:
	int clientFileDescriptor;
	struct sockaddr_in serverAddress;

public:
	ClientSocket();
	~ClientSocket();
	int sendDataToServer(const char *message, int size);
};
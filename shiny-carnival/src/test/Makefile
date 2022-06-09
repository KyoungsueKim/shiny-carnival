NAME = fws_system
SERVER = server
CLIENT1 = client1
CLIENT2 = client2
CC = g++

$(NAME)	:	$(SERVER) $(CLIENT1) $(CLIENT2)

$(SERVER)	:
	g++ -std=c++11 -w -o server server.cpp piezo.cpp pwm.cpp `pkg-config --cflags --libs opencv4` -lpthread

$(CLIENT1) : 
	$(CC) -w -o $(CLIENT1) client1.cpp

$(CLIENT2) : 
	$(CC) -w -o $(CLIENT2) client2.cpp

clean	:
	rm -rf $(SERVER) $(CLIENT1) $(CLIENT2)

re	: clean all
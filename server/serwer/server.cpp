// serwer.cpp
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"

#include <WinSock2.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")

//Checks if error occured and mange to handle it
void check_if_error(int check_value, int expected_err_value, int err_code, const char* err_msg) {
	if (check_value == expected_err_value) {
		printf(err_msg, err_code);
		WSACleanup();
	}
}


int main(int argc, const char **argv)
{
	printf("Server\n\n");

	//create buffer to keep msg
	char buff[1024];
	memset(&buff, 0, sizeof(buff)); //clean buffer

	// Initialize Winsock
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
		printf("WSAStartup failed with error: %d\n", err);

	//set socket address params
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY); //set ip address (in this example INADDR_ANY means that app should listen msg from any ip address)
	sa.sin_port = htons(333); //set port (in this example port 333)


	//Create socket
	SOCKET sock = INVALID_SOCKET;
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	check_if_error(sock, INVALID_SOCKET, WSAGetLastError(), "socket creation failed with error %d\n");

	err = bind(sock, (SOCKADDR *)&sa, sizeof(sa));
	check_if_error(err, SOCKET_ERROR, WSAGetLastError(), "socket binding failed with error %d\n");

	//Listning loop
	int recsize;
	for (;;) {
		recsize = recv(sock, buff, 1024, 0);
		if (recsize < 0)
			fprintf(stderr, "%s\n", strerror(errno));
		
		printf("\nSize: %d, %s", recsize, buff);
	}

	WSACleanup();
	return 0;
}

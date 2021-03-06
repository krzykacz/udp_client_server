//client.cpp

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"

#include <WinSock2.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")


//Checks if error occured and manage to handle it
void check_if_error(int check_value, int expected_err_value, int err_code, const char* err_msg) {
	if (check_value == expected_err_value) {
		printf(err_msg, err_code);
		WSACleanup();
	}
}


int main()
{
	printf("Client\n\n");

	char msg[200] = "Hello World";

	// Initialize Winsock
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
		printf("WSAStartup failed with error: %d\n", err);

	SOCKET sock = INVALID_SOCKET;
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	check_if_error(sock, INVALID_SOCKET, WSAGetLastError(), "socket creation failed with error %d\n");

	//set socket address params
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //set ip address (in this example loopbask address = 127.0.0.1) 
	sa.sin_port = htons(333); //set port (in this example 127.0.0.1::333)


	//Sending loop
	int bytes_send;
	for (;;) {
		printf("Press enter to send.");
		getchar();

		bytes_send = sendto(sock, msg, sizeof msg, 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_in));
		if (bytes_send < 0)
			printf("Błąd wysłania pakietu: %s\n", strerror(errno));
	}
	
	closesocket(sock);

	getchar();
    return 0;
}

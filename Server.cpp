#include <iostream>
#include<winsock2.h>
#include<ws2tcpip.h>


int main()
{
	WSAData wsaData;
	int wsaDatares= WSAStartup(MAKEWORD(2,2),&wsaData);
	if (wsaDatares != 0)
	{
		printf("WSA - ERROR\n");
		return 1;
	}
	else
	{
		printf("WSA - SUCCES\n");
	}
	

	//socket TCP, ipv4
	SOCKET soc = INVALID_SOCKET;
	soc=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(soc==INVALID_SOCKET)
	{ 
		printf("SOCKET - ERROR %d\n",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("SOCKET - SUCCES\n");
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(55555);

	if (bind(soc, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
		printf("BIND - ERROR\n");
		closesocket(soc);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("BIND - SUCCES\n");
	}
		
	if (listen(soc, 1) == SOCKET_ERROR)
	{
		printf("LISTENER - ERROR\n");
	}
	else {
		printf("LISTENER - SUCCES\n");
	
	}

	SOCKET acceptS;
	acceptS= accept(soc, NULL,NULL);

	if (acceptS == INVALID_SOCKET)
	{
		printf("ACCEPT - ERROR\n");
		WSACleanup();
	}
	else
	{
		printf("ACCEPT - SUCCES\n");
	}
	
	char buffer[200];
	int bytes = recv(acceptS, buffer, 200, 0);
	if (bytes < 0)
	{
		printf("RECIVE - ERROR\n");
	}
	else
	{
		printf("RECIVE - SUCCES\n");
	}


	return 0;


}
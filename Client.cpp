#include <iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include <conio.h>
#include<String>
#include <vector>
#include <thread>
using namespace std;


void listening(SOCKET socket);
int main()
{std::vector<std::thread> ThreadVector;
	//pobranie nazwy
	string name;
	char name_tab[20];
	cout<<"ENTER NAME:";
	getline(cin,name);
	strcpy(name_tab,name.c_str());

	

	WSAData wsaData;
	int wsaDatares = WSAStartup(MAKEWORD(2, 2), &wsaData);
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
	SOCKET c_soc = INVALID_SOCKET;
	c_soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_soc == INVALID_SOCKET)
	{
		printf("SOCKET - ERROR %d\n", WSAGetLastError());
		return 1;
	}
	else
	{
		printf("SOCKET - SUCCES\n");
	}


	sockaddr_in c_service;
	c_service.sin_family = AF_INET;
	c_service.sin_addr.s_addr = inet_addr("127.0.0.1");
	c_service.sin_port = htons(55555);

	if (connect(c_soc, (SOCKADDR*)&c_service, sizeof(c_service)) == SOCKET_ERROR) {
		printf("CONNECT - ERROR - %d\n", WSAGetLastError);
		closesocket(c_soc);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("CONNECT - SUCCES\n");
	}
	send(c_soc, name_tab, 200, 0);
	ThreadVector.emplace_back([&](){listening(c_soc);});
while(true)
{
	

	char buffer[200]={0};

	
	cin.getline(buffer, 200);
	int bytes = send(c_soc, buffer, 200, 0);
	if (bytes ==SOCKET_ERROR)
	{
		printf("SEND - ERROR\n");
	}
	

}
	closesocket(c_soc);
	WSACleanup();
	return 0;


}


void listening(SOCKET socket)
{
while (true) {
            char buffer[200] = {0};
            int bytes = recv(socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                
                    printf("RECIVE - ERROR\n");
                
                break;
            } else {
			
                printf("%s\n", buffer);
				
            }
}
closesocket(socket);
}
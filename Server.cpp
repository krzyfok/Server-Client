#include <iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include <vector>
#include <thread>
using namespace std;
//
//funkcja wątki- odieranie tekstu 
//w pętli nasłuchiwanie i dodawanie nowych wątkow dla klientów
//gui- aktywne połacznia-historia-wiadomosc z serwera do wybranego hosta?-osobny wątek na wysyłanie i odbieranie?
// sieć lokalna?
//zapis danych klienta - wiadomosc-data-czas


void communication(SOCKET soc_client);

int main()
{
	std::vector<std::thread> ThreadVector;



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

while(true){
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
	ThreadVector.emplace_back([&](){communication(acceptS);});

}
	
	closesocket(soc);
	WSACleanup();
	return 0;
}

void communication(SOCKET soc_client)
{
while (true) {
            char buffer[200] = {0};
            int bytes = recv(soc_client, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                
                    printf("RECIVE - ERROR\n");
                
                break;
            } else {
                printf("RECIVE : %s\n", buffer);
            }
        }

		closesocket(soc_client);
}
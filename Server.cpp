#include <iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include <vector>
#include <thread>
using namespace std;


//gui- aktywne połacznia-historia-wiadomosc z serwera do wybranego hosta?-osobny wątek na wysyłanie i odbieranie?
// sieć lokalna?
//zapis danych klienta - wiadomosc-data-czas
//przekazywanie wiadomosci do wszytskich hostów połączonych
//wątek do nasłuchiwania u hosta 
void communication(SOCKET socket,vector<SOCKET> &SocketVector);

int main()
{
	vector<thread> ThreadVector;
	vector<SOCKET> SocketVector;


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
	SocketVector.push_back(acceptS);
	ThreadVector.emplace_back([&](){communication(acceptS,SocketVector);});
	
}
	
	closesocket(soc);
	WSACleanup();
	return 0;
}

void communication(SOCKET socket,vector<SOCKET> &SocketVector)
{
	string user_name;
	string separator=" : ";
char buffer[200] = {0};

int bytes = recv(socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                
                    printf("USER_NAME - ERROR\n");
					
                

            } else {
                printf("USER_NAME : %s\n", buffer);
				user_name=buffer;
				
            }
	
while (true) {
            char buffer[200] = {0};
            int bytes = recv(socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                
                    printf("RECIVE - ERROR\n");
                
                break;
            } else {
			
                printf("%s : %s\n",user_name.c_str(), buffer);
				
            }

			memmove(buffer +separator.length()+ user_name.length(), buffer, strlen(buffer) + 1);
			strncpy(buffer,user_name.c_str(),user_name.length());
			  strncpy(buffer + user_name.length(), separator.c_str(), separator.length());
			
		SOCKET s;
			for(int i=0;i<SocketVector.size();i++)
			{
				s=SocketVector[i];
				if(s!=socket) send(s, buffer, sizeof(buffer), 0);
			}
			
        }

		closesocket(socket);
}
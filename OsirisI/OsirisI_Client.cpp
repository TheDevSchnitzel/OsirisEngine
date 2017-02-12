#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Events;
using namespace OsirisI::Networking;
using namespace OsirisI::Manager;


OsirisI_Client::OsirisI_Client(){
}

OsirisI_Client::~OsirisI_Client(){
}


bool OsirisI_Client::Initialize(std::string serverIP, unsigned short port, bool ipV6) {
#ifdef WINDOWS
	SOCKADDR_IN address;
	long rc;

	clientRunning = true;
	WSADATA wsa;
	rc = WSAStartup(MAKEWORD(2, 0), &wsa);

	if(rc != 0){
		printf("Fehler: startWinSock, Code: %d\n", rc);
		return false;
	}
	else{
		printf("WinSock gestartet!\n");
	}

	sSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(sSocket == INVALID_SOCKET){
		printf("Fehler: Socket konnte nicht erstellt werden, Code: %d\n", WSAGetLastError());
		return false;
	}
	else {
		printf("Socket erstellt!\n");
	}

	memset(&address, 0, sizeof(SOCKADDR_IN));
	address.sin_family = AF_INET;
	address.sin_port = htons(port); //Port
	//address.sin_addr.s_addr = inet_addr(serverIP.c_str()); //Zieladresse
	rc = connect(sSocket, (SOCKADDR*)&address, sizeof(SOCKADDR));
	if(rc == SOCKET_ERROR){
		printf("Fehler: connect, Code: %d\n", WSAGetLastError());
		return 1;
	}
	else {
		printf("Verbunden!\n");        
        listenerThread = TaskManager::StartThread([=] () {Receiver(); });
        EventManager::FireQueueEvent(new OsirisI_NetworkStatusEvent(OsirisI_NetworkStatus::CLIENT_STARTED));
		return true;
	}
#endif
	return false;
}

void OsirisI_Client::Receiver(){
	long rc;
	bool abort = false;
	while(!abort && clientRunning){
		char buf[1496];
		rc = recv(sSocket, buf, 1496, 0);

		if(rc == 1496){ //Auswertung
			NetworkFrame* frame;
			frame = (NetworkFrame*) &buf;

			if(frame->action == -1) abort = true;
			else { //AUSWERTUNG
                EventManager::FireAsyncEvent(new OsirisI_NetworkDataEvent(*frame));
            } 
		}
		else if(rc > 0 && rc < 1496){
			abort = true;
			//ERROR!!
			//NETZWERKFRAME BESCHÄDIGT!!!
		}	
		else if( rc == -1){
			abort = true;
		}
	}
	clientRunning = false;
}

bool OsirisI_Client::Send(NetworkFrame networkFrame){
	char* buf;
	buf = (char*) &networkFrame;

	long rc = send(sSocket, buf, 1496, 0);
	if(rc == 1496){
		return true;
	}
	else {
		return false;
	}
	return false;
}
void OsirisI_Client::Close(){
	clientRunning = false;
	listenerThread.join();
	closesocket(sSocket);
	WSACleanup();
}
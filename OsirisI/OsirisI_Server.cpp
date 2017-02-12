#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Events;
using namespace OsirisI::Networking;
using namespace OsirisI::Utilities;
using namespace OsirisI::Manager;

namespace OsirisI {
    namespace Networking {
#ifdef WINDOWS
        struct ConnectedSockets {
            std::mutex lock;
            std::list<SOCKET> connectedSockets;
            ConnectedSockets() {
            }

            ConnectedSockets& operator=(ConnectedSockets const& other){
                this->lock.lock();
                connectedSockets = other.connectedSockets;
                this->lock.unlock();
                return *this;
            }
        };

#endif
#ifdef LINUX
#endif

        OsirisI_Server::OsirisI_Server() {

        }

        OsirisI_Server::~OsirisI_Server() {
        }


        bool OsirisI_Server::Initialize(int pMaxClients, unsigned short pPort, bool pRetryFailedTransmissions){
            maxClients = pMaxClients;
            port = pPort;
            retryFailedTransmissions = pRetryFailedTransmissions;

            if (maxClients <= 0){
                LOG_ERROR("Die maximale Anzahl der Clients muss größer als 0 sein!");
                return false;
            }

            connectedSockets = new ConnectedSockets();

#ifdef WINDOWS
            connectedSockets->connectedSockets = std::list<SOCKET>();
            if (!StartWinSock()) return false;
            if (!CreateAcceptSocket()) return false;
            if (!BindAcceptSocket()) return false;
            if (!SetListenModeAcceptSocket()) return false;
            serverRunning = true;
            Acceptor();
            return true;
#endif
#ifdef LINUX
#endif

        }

        bool OsirisI_Server::StartWinSock(){
#ifdef WINDOWS
            //WinSock starten
            WSADATA wsa;
            rc = WSAStartup(MAKEWORD(2, 0), &wsa);

            if (rc != 0){
                LOG_ERROR("Fehler: startWinSock, Code: " << rc);
                return false;
            }
            LOG_INFO("WinSock gestartet!");
            return true;
#endif
#ifdef LINUX
#endif
        }
        bool OsirisI_Server::CreateAcceptSocket(){
#ifdef WINDOWS
            //Socket erstellen
            acceptSocket = socket(AF_INET, SOCK_STREAM, 0);

            if (acceptSocket == INVALID_SOCKET) {
                LOG_ERROR("Fehler: Socket konnte nicht erstellt werden, Code: " << WSAGetLastError());
                return false;
            }
            LOG_INFO("Socket erstellt!");
            return true;
#endif
#ifdef LINUX
#endif
        }
        bool OsirisI_Server::BindAcceptSocket(){
#ifdef WINDOWS
            memset(&address, 0, sizeof(SOCKADDR_IN));
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            address.sin_addr.s_addr = ADDR_ANY;
            rc = bind(acceptSocket, (SOCKADDR*)&address, sizeof(SOCKADDR_IN));
            if (rc == SOCKET_ERROR){
                LOG_ERROR("Fehler: bind, Code: " << WSAGetLastError());
                return false;
            }
            LOG_INFO("Socket an Port " << port << " gebunden!");
            return true;
#endif
#ifdef LINUX
#endif
        }
        bool OsirisI_Server::SetListenModeAcceptSocket(){
#ifdef WINDOWS
            rc = listen(acceptSocket, maxClients);
            if (rc == SOCKET_ERROR){
                LOG_ERROR("Fehler: listen, Code: " << WSAGetLastError());
                return false;
            }
            LOG_INFO("Modus listen für acceptSocket aktiviert...");
            return true;
#endif
#ifdef LINUX
#endif
        }
        bool OsirisI_Server::Acceptor(){
#ifdef WINDOWS
            listener = TaskManager::StartThread([&]() { // & - by reference
                while (serverRunning){
                    SOCKET connectedSocket = accept(acceptSocket, NULL, NULL);
                    if (connectedSocket == INVALID_SOCKET){
                        LOG_ERROR("Fehler: accept, Code: " << WSAGetLastError());
                        break;
                    }
                    else {
                        LOG_INFO("Neue Verbindung wurde akzeptiert!");

                        //Starte neuen Thread der den angenommenen Socket abhört
                        listOfThreads.push_back(std::thread(&OsirisI::Networking::OsirisI_Server::Receiver, this, connectedSocket));
                    }
                }
            });
#endif
#ifdef LINUX
#endif
            return true;
        }

#ifdef WINDOWS
        void OsirisI_Server::Receiver(SOCKET socket){
            connectedSockets->lock.lock();
            connectedSockets->connectedSockets.push_back(socket);
            connectedSockets->lock.unlock();

            long rc;
            bool abort = false;
            while (!abort && serverRunning){
                char buf[1496];
                rc = recv(socket, buf, 1496, 0);

                if (rc == 1496){ //Auswertung
                    NetworkFrame* frame;
                    frame = (NetworkFrame*)&buf;

                    if (frame->action == -1) abort = true;
                    else {//AUSWERTUNG
                        EventManager::FireAsyncEvent(new OsirisI_NetworkDataEvent(*frame));
                    }
                }
                else if (rc > 0 && rc < 1496) { //ERROR!!
                    abort = true;
                    EventManager::FireAsyncEvent(new OsirisI_NetworkErrorEvent(OsirisI_NetworkErrors::DAMAGED_FRAME)); //NETZWERKFRAME BESCHÄDIGT!!!

                    if (retryFailedTransmissions){
                        //Wenn nötig neu anfordern
                    }
                }
                else if (rc == -1){
                    abort = true;
                }
            }

            connectedSockets->lock.lock();
            std::list<SOCKET>::iterator it;
            for (it = connectedSockets->connectedSockets.begin(); it != connectedSockets->connectedSockets.end(); ++it){
                if (socket == *it){
                    connectedSockets->connectedSockets.erase(it);
                    it = connectedSockets->connectedSockets.end();
                }
            }
            connectedSockets->lock.unlock();
        }
#endif
#ifdef LINUX
#endif

        bool OsirisI_Server::Stop(){
            if(!serverRunning) return true;

            serverRunning = false;            
            LOG_INFO("CloseSocket - Acceptor: " << closesocket(acceptSocket) << " | " << WSAGetLastError());
            
            connectedSockets->lock.lock();
            while (connectedSockets->connectedSockets.size() > 0) {
                closesocket(connectedSockets->connectedSockets.front());
                connectedSockets->connectedSockets.pop_front();
            }
            connectedSockets->lock.unlock();

            for (unsigned int i = 0; i < listOfThreads.size(); i++) listOfThreads[i].join();
            return true;
        }
    }
}

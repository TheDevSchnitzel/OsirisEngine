#ifndef OSIRISI_SERVER_HPP_
#define OSIRISI_SERVER_HPP_

namespace OsirisI {
    namespace Networking {
        struct ConnectedSockets;

        class OsirisI_Server {
            public:
            OsirisI_Server();
            ~OsirisI_Server();

            bool Initialize(int pMaxClients, unsigned short pPort, bool pRetryFailedTransmissions = false);
            bool Stop();

            private:
            int maxClients;
            short port;
            bool serverRunning;
            bool retryFailedTransmissions;
            std::thread listener;

#ifdef WINDOWS
            long rc;
            SOCKET acceptSocket;
            SOCKADDR_IN address;

            void Receiver(SOCKET socket);
#endif
#ifdef LINUX
#endif

            ConnectedSockets* connectedSockets;
            std::vector<std::thread> listOfThreads;


            bool StartWinSock();
            bool CreateAcceptSocket();
            bool BindAcceptSocket();
            bool SetListenModeAcceptSocket();
            bool Acceptor();
        };
    }
}

#endif /* OSIRISI_SERVER_HPP_ */

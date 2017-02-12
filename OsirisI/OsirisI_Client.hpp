#ifndef OSIRISI_CLIENT_HPP_
#define OSIRISI_CLIENT_HPP_

namespace OsirisI {
	namespace Networking {
		class OsirisI_Client {
			private:

            public:
            OsirisI_Client();
            ~OsirisI_Client();

            bool Initialize(std::string serverIP, unsigned short port, bool ipV6 = false);
            void Close();
            bool Send(NetworkFrame networkFrame);

            private:
            void Receiver();

            std::thread listenerThread;
            bool clientRunning;
            SOCKET sSocket;
        };
	}
}

#endif /* OSIRISI_CLIENT_HPP_ */

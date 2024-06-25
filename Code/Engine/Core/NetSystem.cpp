#include "Engine/Core/NetSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


//define to disable net system
#if !defined(ENGINE_DISABLE_NETWORK)


//global net system pointer
NetSystem* g_theNetSystem = nullptr;


//
//public game flow functions
//
void NetSystem::Startup()
{
	m_sendBuffer = new char[m_config.m_sendBufferSize];
	m_recvBuffer = new char[m_config.m_recvBufferSize];
	
	if (m_config.m_mode == NetSystemMode::CLIENT)
	{
		//startup Windows sockets
		WSADATA data;
		int result = WSAStartup(MAKEWORD(2, 2), &data);
		if (result != 0)
		{
			ERROR_AND_DIE("WSAStartup returned error!");
		}

		//create client socket
		m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		//set blocking mode
		unsigned long blockingMode = 1;
		result = ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
		if (result != 0)
		{
			ERROR_AND_DIE("Couldn't set blocking mode to non-blocking!");
		}

		//get host address
		IN_ADDR addr;
		result = inet_pton(AF_INET, "127.0.0.1", &addr);
		if (result != 1)
		{
			ERROR_AND_DIE("Host address initialization error!");
		}
		m_hostAddress = ntohl(addr.S_un.S_addr);

		//get host port
		m_hostPort = static_cast<unsigned short>(atoi("27015"));
	}
	else if (m_config.m_mode == NetSystemMode::SERVER)
	{
		//startup Windows sockets
		WSADATA data;
		int result = WSAStartup(MAKEWORD(2, 2), &data);
		if (result != 0)
		{
			ERROR_AND_DIE("WSAStartup returned error!");
		}
		
		//create listen socket
		m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		//set blocking mode
		unsigned long blockingMode = 1;
		result = ioctlsocket(m_listenSocket, FIONBIO, &blockingMode);
		if (result != 0)
		{
			ERROR_AND_DIE("Couldn't set blocking mode to non-blocking!");
		}

		//get host port
		m_hostAddress = INADDR_ANY;
		m_hostPort = static_cast<unsigned short>(atoi("27015"));

		//bind listen socket to port
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
		addr.sin_port = htons(m_hostPort);
		result = bind(m_listenSocket, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
		if (result != 0)
		{
			ERROR_AND_DIE("Couldn't bind listen socket to port!");
		}

		//listen for connections to accept
		result = listen(m_listenSocket, SOMAXCONN);
		if (result != 0)
		{
			ERROR_AND_DIE("Error when listening for connections!");
		}
	}
}


void NetSystem::BeginFrame()
{
	if (m_config.m_mode == NetSystemMode::CLIENT)
	{
		//attempt to connect if we haven't already
		if (!m_connected)
		{
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
			addr.sin_port = htons(m_hostPort);
			int result = connect(m_clientSocket, (sockaddr*)(&addr), static_cast<int>(sizeof(addr)));
			if (result == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();
				if (errorCode == WSAEWOULDBLOCK)
				{
					return;
				}
				/*else
				{
					ERROR_AND_DIE("Fatal error in client connect function!");
				}*/
			}
		}

		//check if our connection attempt failed or completed
		fd_set writeSockets;
		FD_ZERO(&writeSockets);
		FD_SET(m_clientSocket, &writeSockets);
		fd_set errorSockets;
		FD_ZERO(&errorSockets);
		FD_SET(m_clientSocket, &errorSockets);
		timeval waitTime = {};
		int result = select(0, NULL, &writeSockets, &errorSockets, &waitTime);
		if (result == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if(errorCode == WSAEWOULDBLOCK)
			{
				return;
			}
			else
			{
				ERROR_AND_DIE("Fatal error in client select function!");
			}
		}
		else if (result > 0 && FD_ISSET(m_clientSocket, &errorSockets))
		{
			//connection failed if we're here
			return;
		}
		else if (result > 0 && FD_ISSET(m_clientSocket, &writeSockets))
		{
			//if(!m_connected) g_theDevConsole->AddLine(DevConsole::COLOR_WARNING, "Connected!");
			m_connected = true;

			//send on client socket
			int sendQueueSize = static_cast<int>(m_sendQueue.size());
			for (int sendIndex = 0; sendIndex < sendQueueSize; sendIndex++)
			{
				std::string& sendString = m_sendQueue[sendIndex];
				if (sendString.size() > m_config.m_sendBufferSize)
				{
					ERROR_AND_DIE("Send buffer too small to send data!");
				}
				strcpy_s(m_sendBuffer, static_cast<size_t>(m_config.m_sendBufferSize), sendString.c_str());

				int sendBufferSize = static_cast<int>(strlen(m_sendBuffer) + 1);
				result = send(m_clientSocket, m_sendBuffer, sendBufferSize, 0);
				if (result == SOCKET_ERROR)
				{
					int errorCode = WSAGetLastError();
					if (errorCode != WSAEWOULDBLOCK)
					{
						ERROR_AND_DIE("Fatal error sending from client!");
					}
				}

				DebuggerPrintf("Data sent: %s  --  Data size: %i", sendString.c_str(), sendBufferSize);
			}
			m_sendQueue.clear();

			//receive on client socket
			bool somethingReceived = false;
			do
			{
				somethingReceived = false;
				result = recv(m_clientSocket, m_recvBuffer, m_config.m_recvBufferSize, 0);
				if (result == SOCKET_ERROR)
				{
					int errorCode = WSAGetLastError();
					if (errorCode != WSAEWOULDBLOCK)
					{
						if (errorCode == WSAECONNRESET || errorCode == WSAECONNABORTED)
						{
							m_connected = false;
						}
						else
						{
							std::string errorMessage = Stringf("Fatal error receiving on client! Error code: %i", errorCode);
							ERROR_AND_DIE(errorMessage);
						}
					}
					/*else
					{
						ERROR_RECOVERABLE("Done receiving on client!");
					}*/
				}
				else if (result == 0)
				{
					//ERROR_RECOVERABLE("Result of 0 from client recv!");
					//they disconnect, disconnect ourselves
					m_connected = false;
				}
				else
				{
					somethingReceived = true;
				}

				if (somethingReceived)
				{
					//parse out received string
					for (int charIndex = 0; charIndex < result; charIndex++)
					{
						char recvChar = m_recvBuffer[charIndex];
						if (recvChar != '\0')
						{
							m_recvRemainder.push_back(recvChar);
						}
						else
						{
							if (m_recvRemainder.size() > 1)
							{
								HandleReceivedString();
							}
							m_recvRemainder.clear();
						}
					}
				}

			} while (somethingReceived);
		}
	}
	else if (m_config.m_mode == NetSystemMode::SERVER)
	{
		//check for connection to accept if we don't have one
		if (!m_connected)
		{
			m_clientSocket = accept(m_listenSocket, NULL, NULL);
			if (m_clientSocket == ~0ull)
			{
				int errorCode = WSAGetLastError();
				if (errorCode == WSAEWOULDBLOCK)
				{
					return;
				}
				else
				{
					ERROR_AND_DIE("Fatal error in server accepting connection from listen socket!");
				}
			}

			//set to non-blocking mode
			unsigned long blockingMode = 1;
			int result = ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
			if (result != 0)
			{
				ERROR_AND_DIE("Couldn't set blocking mode to non-blocking!");
			}

			//g_theDevConsole->AddLine(DevConsole::COLOR_WARNING, "Connected!");
		}

		m_connected = true;

		//send on client socket
		int sendQueueSize = static_cast<int>(m_sendQueue.size());
		for (int sendIndex = 0; sendIndex < sendQueueSize; sendIndex++)
		{

			std::string& sendString = m_sendQueue[sendIndex];
			if (sendString.size() > m_config.m_sendBufferSize)
			{
				std::string sendBufferError = Stringf("Send buffer too small to send data! Send buffer size: %i", m_config.m_sendBufferSize);
				ERROR_AND_DIE(sendBufferError);
			}
			std::string sendBufferError = Stringf("Send buffer too small to send data! Send buffer size: %i", m_config.m_sendBufferSize);

			strcpy_s(m_sendBuffer, static_cast<size_t>(m_config.m_sendBufferSize), sendString.c_str());

			int sendBufferSize = static_cast<int>(strlen(m_sendBuffer) + 1);
			int result = send(m_clientSocket, m_sendBuffer, sendBufferSize, 0);

			if (result == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();
				if (errorCode != WSAEWOULDBLOCK)
				{
					ERROR_AND_DIE("Fatal error sending from server!");
				}
			}

			DebuggerPrintf("Data sent: %s  --  Data size: %i", sendString.c_str(), sendBufferSize);
		}
		m_sendQueue.clear();

		//receive on client socket
		bool somethingReceived = false;
		do 
		{
			somethingReceived = false;
			int result = recv(m_clientSocket, m_recvBuffer, m_config.m_recvBufferSize, 0);
			if (result == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();
				if (errorCode != WSAEWOULDBLOCK)
				{
					if (errorCode == WSAECONNRESET || errorCode == WSAECONNABORTED)
					{
						m_connected = false;
					}
					else
					{
						std::string errorMessage = Stringf("Fatal error receiving on server! Error code: %i", errorCode);
						ERROR_AND_DIE(errorMessage);
					}
				}
				/*else
				{
					ERROR_RECOVERABLE("Done receiving on server!");
				}*/
			}
			else if (result == 0)
			{
				//ERROR_RECOVERABLE("Result of 0 from server recv!");
				//they disconnect, disconnect ourselves
				m_connected = false;
			}
			else
			{
				somethingReceived = true;
			}

			if (somethingReceived)
			{
				//parse out received string
				for (int charIndex = 0; charIndex < result; charIndex++)
				{
					char recvChar = m_recvBuffer[charIndex];
					if (recvChar != '\0')
					{
						m_recvRemainder.push_back(recvChar);
					}
					else
					{
						if (m_recvRemainder.size() > 1)
						{
							HandleReceivedString();
						}
						m_recvRemainder.clear();
					}
				}
			}
		} while (somethingReceived);
	}
}


void NetSystem::Shutdown()
{
	if (m_config.m_mode == NetSystemMode::CLIENT)
	{
		//close all open sockets
		closesocket(m_clientSocket);

		//shutdown windows sockets
		WSACleanup();
	}
	else if (m_config.m_mode == NetSystemMode::SERVER)
	{
		//close all open sockets
		closesocket(m_clientSocket);
		closesocket(m_listenSocket);

		//shutdown windows sockets
		WSACleanup();
	}

	delete m_sendBuffer;
	delete m_recvBuffer;
}


void NetSystem::HandleReceivedString()
{
	g_theDevConsole->Execute(m_recvRemainder, false);

	/*if (m_config.m_mode == NetSystemMode::SERVER)
	{
		std::string echoMessage = Stringf("Echo Message=\"Server executed remote command: %s\"", m_recvRemainder.c_str());
		TrimString(echoMessage, '\0');
		m_sendQueue.emplace_back(echoMessage);
	}*/
}

#endif

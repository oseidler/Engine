#pragma once
#include "Engine/Core/EngineCommon.hpp"


enum class NetSystemMode
{
	SERVER,
	CLIENT,
	NONE
};


struct NetSystemConfig
{
	NetSystemMode m_mode = NetSystemMode::NONE;
	std::string   m_hostAddress = "";
	unsigned int  m_sendBufferSize = 0;
	unsigned int  m_recvBufferSize = 0;
};


class NetSystem
{
//public member functions
public:
	//constructor
	NetSystem(NetSystemConfig config) : m_config(config) {}

	//game flow functions
	void Startup();
	void BeginFrame();
	void EndFrame() {}
	void Shutdown();

	void HandleReceivedString();

//public member variables
public:
	NetSystemConfig m_config;

	uintptr_t m_clientSocket = ~0ull;
	uintptr_t m_listenSocket = ~0ull;
	unsigned long m_hostAddress = 0;
	unsigned short m_hostPort = 0;

	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;

	Strings m_sendQueue;

	bool m_connected = false;

	std::string m_recvRemainder = "";
};


//external variable declaration
extern NetSystem* g_theNetSystem;

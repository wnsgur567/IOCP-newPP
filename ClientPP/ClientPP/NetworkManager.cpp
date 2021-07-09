#include "base.h"

std::unique_ptr<NetworkManagerClient> Singleton<NetworkManagerClient>::sInstance;



bool NetworkManagerClient::StaticInitialize()
{
	sInstance.reset(new NetworkManagerClient());
	return true;
}

bool NetworkManagerClient::Init(u_short inPort, bool isInNonBlock)
{
	//소켓 생성
	m_sock = SocketUtil::CreateTCPSocket();
	if (m_sock == nullptr)
	{
		return false;
	}

	// listen sock를 논블록으로
	if (false == m_sock->SetNonBlockingMode(isInNonBlock))
		return false;

	// connect
	if (false == m_sock->Connect(m_serveraddr))
		return false;

	// debug
	m_serveraddr.Print("Connet to Server [IP] : %s / [Port] : %d");	

	return true;
}

NetworkManagerClient::~NetworkManagerClient()
{
	SocketUtil::CleanUp();
}

bool NetworkManagerClient::Initialize()
{
	// wsa init
	if (false == SocketUtil::Init())
		return false;

	if (false == NetworkManagerClient::Init(SERVERPORT, false))
		return false;

	return true;
}

void NetworkManagerClient::Finalize()
{
}

bool NetworkManagerClient::DoFrame()
{
	return true;
}

TCPSocketPtr NetworkManagerClient::GetSockPtr() const
{
	return m_sock;
}

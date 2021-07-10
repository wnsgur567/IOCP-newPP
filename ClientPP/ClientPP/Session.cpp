#include "base.h"

ClientSession::ClientSession()
	: m_state(ESessionState::None), m_pSock(nullptr), m_addr(),
	m_pRecvPacket(nullptr), m_pSendPacket(nullptr)
{
	m_pRecvPacket = PacketManager::sInstance->GetRecvPacketFromPool();
}

ClientSession::~ClientSession()
{
}

ESessionState ClientSession::GetState()
{
	return m_state;
}

TCPSocketPtr ClientSession::GetSockPtr()
{
	return m_pSock;
}

void ClientSession::SetState(ESessionState inState)
{
	m_state = inState;
}

bool ClientSession::Recv()
{
	return NetworkManagerClient::Recv(m_pSock, m_pRecvPacket);
}

bool ClientSession::Send()
{
	return NetworkManagerClient::Send(m_pSock, m_pSendPacket);
}

ClientSessionPtr ClientSession::CreateSession()
{
	return std::make_shared<ClientSession>();
}

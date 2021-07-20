#include "base.h"

SessionBase::SessionBase()
	: m_pSock(nullptr), m_addr(),
	m_newRecvID(1), m_newSendID(1)
{
}

SessionBase::~SessionBase()
{
}

TCPSocketPtr SessionBase::GetSockPtr()
{
	return m_pSock;
}

SocketAddress SessionBase::GetSockAddress()
{
	return m_addr;
}

bool SessionBase::IsDuplicatedPacket(const pid_t inID)
{
	if (inID < m_newRecvID)
		return true;
	return false;
}

SessionBase::pid_t SessionBase::CountingRecvID()
{
	pid_t id = m_newRecvID;
	++m_newRecvID;
	return id;
}

SessionBase::pid_t SessionBase::CountingSendID()
{
	pid_t id = m_newSendID;
	++m_newSendID;
	return id;
}

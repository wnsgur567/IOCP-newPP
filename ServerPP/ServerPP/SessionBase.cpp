#include "base.h"


SessionBase::SessionBase()
	: m_pSock(nullptr),
	m_newSendID(1), m_newRecvID(1)
{
}

SessionBase::~SessionBase()
{
}

TCPSocketPtr SessionBase::GetSockPtr()
{
	return m_pSock;
}
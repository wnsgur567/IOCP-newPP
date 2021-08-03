#include "base.h"


void SessionBase::SetID(id_t inID)
{
	m_id = inID;
}

SessionBase::SessionBase()
	: m_id(0),
	m_pSock(nullptr),
	m_newSendID(1), m_newRecvID(1)
{
}

SessionBase::~SessionBase()
{
}

SessionBase::id_t SessionBase::GetID()
{
	return m_id;
}


TCPSocketPtr SessionBase::GetSockPtr()
{
	return m_pSock;
}
#include "base.h"


void SessionBase::SetID(id_t inID)
{
	m_id = inID;
}

SessionBase::SessionBase()
	: m_id(0), m_state(ESessionState::None),
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

ESessionState SessionBase::GetState()
{
	return m_state;
}

TCPSocketPtr SessionBase::GetSockPtr()
{
	return m_pSock;
}

void SessionBase::SetState(ESessionState inState)
{
	m_state = inState;
}
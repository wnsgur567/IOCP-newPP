#include "base.h"

Session::Session()
	: m_state(ESessionState::Sign),
	m_pRecvPacket(nullptr), m_pSendPacket(nullptr)
{
}

Session::~Session()
{
}

ESessionState Session::GetState()
{
	return m_state;
}


void Session::SetState(ESessionState inState)
{
	m_state = inState;
}


bool Session::Recv()
{

	return true;
}

bool Session::Send()
{
	return true;
}




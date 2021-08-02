#include "base.h"


void IOCPSession::Initialze()
{
	m_recvPacket = PacketManager::sInstance->GetRecvPacketFromPool();
}

IOCPSession::IOCPSession()
	: IOCPSessionBase(), m_isSigned(false)
{
}

bool IOCPSession::Recv()
{
	//IOCPNetworkManager::sInstance->RecvAsync()

	return false;
}

bool IOCPSession::Send()
{
	return false;
}

bool IOCPSession::OnCompleteRecv()
{
	return false;
}

bool IOCPSession::OnCompleteSend()
{
	return false;
}

void IOCPSession::SetSigned(bool b)
{
	m_isSigned = b;
}

bool IOCPSession::IsSigned() const
{
	return m_isSigned;
}
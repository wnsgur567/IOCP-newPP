#include "IOCPBase_RootHeader.h"

namespace IOCP_Base
{

	IOCPSessionBase::IOCPSessionBase()
		: m_pSock(nullptr),
		m_newSendID(1), m_newRecvID(1)
	{
		m_pRecvPacket = NetBase::PacketManager::sInstance->GetRecvPacketFromPool();
	}

	void IOCPSessionBase::SetEndPoint(NetBase::TCPSocketPtr inpSock, NetBase::SocketAddress inAddress)
	{
		m_pSock = inpSock;
		m_addr = inAddress;
	}

	NetBase::TCPSocketPtr IOCPSessionBase::GetSockPtr()
	{
		return m_pSock;
	}
}

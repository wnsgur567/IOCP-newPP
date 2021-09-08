#include "IOCPBase_RootHeader.h"

namespace IOCP_Base
{

	IOCPSessionBase::IOCPSessionBase()
	{
		m_pRecvPacket = NetBase::PacketManager::sInstance->GetRecvPacketFromPool();
	}

	void IOCPSessionBase::SetEndPoint(NetBase::TCPSocketPtr inpSock, NetBase::SocketAddress inAddress)
	{
		m_pSock = inpSock;
		m_addr = inAddress;
	}
}

#include "IOCPBase_RootHeader.h"

namespace IOCP_Base
{

	SessionBase::SessionBase()
		: m_pSock(nullptr),
		m_newSendID(1), m_newRecvID(1)
	{
	}

	SessionBase::~SessionBase()
	{
		printf("!Session\n");
	}

	NetBase::TCPSocketPtr SessionBase::GetSockPtr()
	{
		return m_pSock;
	}
}
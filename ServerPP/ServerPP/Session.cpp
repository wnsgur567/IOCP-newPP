#include "base.h"


void Session::SetID(id_t inID)
{
	m_id = inID;
}

Session::Session()
	: m_id(0), m_state(ESessionState::None),
	m_pSock(nullptr), m_pRecvPacket(nullptr), m_pSendPacket(nullptr),
	m_newSendID(0), m_newRecvID(0)
{
}

Session::~Session()
{
}

Session::id_t Session::GetID()
{
	return m_id;
}

ESessionState Session::GetState()
{
	return m_state;
}

TCPSocketPtr Session::GetSockPtr()
{
	return m_pSock;
}

void Session::SetState(ESessionState inState)
{
	m_state = inState;
}



IOCPSession::IOCPSession()
	: Session()
{
}

bool IOCPSession::Recv()
{
	if (false == IOCPNetworkManager::RecvAsync(m_pSock, m_pRecvPacket))
	{
		return false;
	}
	return true;
}

bool IOCPSession::Send()
{
	m_sendQueue.push(m_pSendPacket);

	if (m_sendQueue.size() == 1)
	{
		if (false == IOCPNetworkManager::SendAsync(m_pSock, m_pSendPacket))
		{
			return false;
		}
	}
	return true;
}

IOCPSessionPtr IOCPSession::CreateSession()
{
	IOCPSessionPtr newSession = std::make_shared<IOCPSession>();

	// ...

	return newSession;
}

bool IOCPSession::OnAccepted(TCPSocketPtr inpSock, SocketAddress inAddress)
{
	m_pSock = inpSock;
	m_addr = inAddress;
	SocketUtil::LinkIOCPThread(inpSock, m_id);

	m_state = ESessionState::Sign;

	return true;
}

bool IOCPSession::OnDisconnected()
{
	m_state = ESessionState::Disconnected;

	return true;
}

bool IOCPSession::OnCompleteRecv(RecvPacketPtr inRecvPacket)
{
	InputMemoryStreamPtr pStream = inRecvPacket->GetStream();

	switch (m_state)
	{
	case ESessionState::None:
	{

	}
	break;
	case ESessionState::Sign:
	{
		SignManager::ProcessResult result = SignManager::sInstance->StreamProcess(shared_from_this(), pStream);
		m_pSendPacket = std::make_shared<SendPacket>(result.pStream);
		Send();
	}
	break;
	case ESessionState::Disconnected:
	{

	}
	break;
	}

	return true;
}

bool IOCPSession::OnCompleteSend(SendPacketPtr)
{
	return true;
}

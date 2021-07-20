#include "base.h"

Implementation_sInstance(IOCPNetworkManager);

bool IOCPNetworkManager::Initialize(LPVOID arg)
{
	IOCPNetworkManagerBase<IOCPNetworkManager>::Initialize(arg);

	if (false == SignManager::StaticInit())
		return false;
	if (false == SignManager::sInstance->Initialize(nullptr))
		return false;

	return true;
}

void IOCPNetworkManager::Finalize()
{
	IOCPNetworkManagerBase<IOCPNetworkManager>::Finalize();
}

bool IOCPNetworkManager::DoFrame()
{
	SocketAddress client_addr;
	TCPSocketPtr client_sock = m_pListenSock->Accept(client_addr);

	if (nullptr == client_sock)
		return false;

	AcceptPacketPtr pAcceptPacket = PacketManager::sInstance->GetAcceptPacketFromPool();
	pAcceptPacket->Set(client_sock, client_addr);

	PostQueuedCompletionStatus(
		*IOCPNetworkManager::sInstance->m_pHcp,
		1,
		(ULONG_PTR)&pAcceptPacket,
		&pAcceptPacket->GetOverlappedRef().overlapped);

	return true;
}

void IOCPNetworkManager::OnAccepted(TCPSocketPtr inpSock, SocketAddress inAddress)
{
	IOCPSessionPtr pSession = IOCPSessionManager::sInstance->CreateSession<IOCPSession>();
	pSession->m_pSock = inpSock;
	pSession->m_addr = inAddress;

	SocketUtil::LinkIOCPThread(inpSock, *m_pHcp);

	pSession->SetState(ESessionState::Sign);

	// 최초 recv
	RecvPacketPtr pPacket = PacketManager::sInstance->GetRecvPacketFromPool();;
	sInstance->RecvAsync(pSession, pPacket);
}

void IOCPNetworkManager::OnDisconnected(IOCPSessionBasePtr inpSession)
{



}

void IOCPNetworkManager::OnCompleteRecv(IOCPSessionBasePtr inpSession, RecvPacketPtr inpPacket)
{
	IOCPSessionPtr pSession = std::static_pointer_cast<IOCPSession>(inpSession);

	InputMemoryStreamPtr pStream = inpPacket->GetStream();
	ESessionState state = pSession->GetState();
	switch (state)
	{
	case ESessionState::None:
		break;
	case ESessionState::Sign:
	{
		// QQ : stream 처리를 밖에서 할 것인가 안에서 할 것인가?
		auto result = SignManager::sInstance->StreamProcess(pStream, pSession->IsSigned());

		SendPacketPtr pSendPacket = PacketManager::sInstance->GetSendPacketFromPool();
		pSendPacket->SetStream(result.pStream);

		if (false == SendAsync(pSession, pSendPacket))
		{
			// ...
		}
	}
	break;
	case ESessionState::Disconnected:
		break;
	}


}

void IOCPNetworkManager::OnCompleteSend(IOCPSessionBasePtr inpSession, SendPacketPtr inpPacket)
{

}


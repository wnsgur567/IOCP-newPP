#include "base.h"
#include "IOCPNetworkManager.h"

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
		NULL,
		&pAcceptPacket->GetOverlappedPtr()->overlapped);

	return true;
}

bool IOCPNetworkManager::OnRecved(SOCKET inSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer, DWORD inCbTransferred)
{
	PacketBase::EPacketState result = CompleteRecv(inSock, inpRecvPacket, inPointer, inCbTransferred);

	switch (result)
	{
	case PacketBase::EPacketState::Error:
		return false;
	case PacketBase::EPacketState::End:
		return false;
	case PacketBase::EPacketState::InComplete:
		return false;
	case PacketBase::EPacketState::Completed:
		// �Ϸ�� ��� �ð��� ���
		inpRecvPacket->RecordRecvTime();
		break;
	}

	return true;
}

bool IOCPNetworkManager::OnSended(SOCKET inSock, SendPacketPtr inpSendPacket, VoidPtr inPointer, DWORD inCbTransferred)
{
	// send �Ϸ� Ȯ��
	PacketBase::EPacketState result = CompleteSend(inSock, inpSendPacket, inPointer, inCbTransferred);

	switch (result)
	{
	case PacketBase::EPacketState::Error:
		return false;
	case PacketBase::EPacketState::End:
		return false;
	case PacketBase::EPacketState::InComplete:
		return false;
	case PacketBase::EPacketState::Completed:
		break;
	}

	return true;
}

bool IOCPNetworkManager::OnCompleteRecv(VoidPtr inPointer)
{
	IOCPSessionBasePtr pSession = std::static_pointer_cast<IOCPSessionBase>(inPointer);

	if (false == pSession->OnCompleteRecv())
		return false;
	return true;
}

bool IOCPNetworkManager::OnCompleteSend(VoidPtr inPointer)
{
	IOCPSessionPtr pSession = std::static_pointer_cast<IOCPSession>(inPointer);

	if (false == pSession->OnCompleteSend())
		return false;
	return true;
}

void IOCPNetworkManager::OnAccepted(AcceptPacketPtr inpPacket)
{
	// ���ο� Ŭ���̾�Ʈ ���� ����
	IOCPSessionPtr pSession = IOCPSessionManager::sInstance->CreateSession<IOCPSession>();
	pSession->m_pSock = inpPacket->GetPSock();
	pSession->m_addr = inpPacket->GetAddr();

	// thread ����
	SocketUtil::LinkIOCPThread(pSession->m_pSock->GetSock(), *m_pHcp);

	// ��Ŷ ȸ��
	PacketManager::sInstance->RetrieveAcceptPacket(inpPacket);

	pSession->Initialze();
	// ���� recv
	if (false == pSession->Recv())
	{
		// TODO : disconnected or return false
	}
}

void IOCPNetworkManager::OnDisconnected(VoidPtr)
{
	// TODO : disconnected
}
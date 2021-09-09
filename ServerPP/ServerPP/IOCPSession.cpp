#include "IOCPNet_RootHeader.h"
#include <handleapi.h>

IOCP_Base::IOCPSessionBasePtr IOCPSession::CreateSession()
{
	IOCP_Base::IOCPSessionBasePtr retPointer;
	retPointer.reset(new IOCPSession());
	retPointer->Initialze();
	return retPointer;
}

void IOCPSession::Initialze()
{
	// TODO : ��� state �Ҵ�
	auto pSignState = SignState::Create(shared_from_this());	 
	
	// ù ������ �α��� ���·� ������
	m_current_state = pSignState;
}

IOCPSession::IOCPSession()
	: IOCPSessionBase(),
	m_isSigned(false)
{

}

bool IOCPSession::Recv()
{
	int flag;
	socklen_t size = sizeof(flag);
	int retval = getsockopt(m_pSock->GetSock(), SOL_SOCKET, SO_REUSEADDR, (char*)&flag, &size);
	if (retval == EBADF)
	{
		printf("EBADF\n");
	}
	else if (retval == EFAULT)
	{
		printf("EFAULT\n");
	}
	else if (retval == EINVAL)
	{
		printf("EINVAL\n");
	}
	else if (ENOPROTOOPT)
	{
		printf("ENOPROTOOPT\n");
	}
	else if (ENOTSOCK)
	{
		printf("ENOTSOCK\n");
	}
	else
	{
		printf("����\n");
	}


	if (false == IOCP_Base::IOCPNetworkManager::sInstance->RecvAsync(
		m_pSock->GetSock(),
		m_pRecvPacket,
		shared_from_this()))
		return false;

	return true;
}

bool IOCPSession::Send(NetBase::OutputMemoryStreamPtr pStream)
{
	auto pSendPacket = NetBase::PacketManager::sInstance->GetSendPacketFromPool();

	//
	pSendPacket->Packing(m_newSendID, pStream);
	++m_newSendID;

	// send queue �� �߰�
	m_sendPacketQueue.push(pSendPacket);

	// ���� �������� send �۾��� ���ٸ� (�ִٸ� ������)
	if (m_sendPacketQueue.size() == 1)
	{	// ��� ����
		if (false == IOCP_Base::IOCPNetworkManager::sInstance->SendAsync(
			m_pSock->GetSock(),
			m_sendPacketQueue.front(),
			shared_from_this()))
			return false;
	}

	return true;
}

bool IOCPSession::OnCompleteRecv()
{
	NetBase::PacketBase::packetId_t packet_id;
	NetBase::InputMemoryStreamPtr pStream;

	m_pRecvPacket->UnPackging(packet_id, pStream);

#ifdef __DEBUG
	// �Ϸ�� ��Ŷ�� id�� Ȯ��	
	printf("recv_id : (%lu, %lu)\n", packet_id, m_newRecvID);
#endif // __DEBUG

	if (packet_id < m_newRecvID)
	{	// �̹� ó�� �Ϸ�� �ߺ� ��Ŷ ���
		return true;
	}

	++m_newRecvID;

	/*--------- data process     ----------*/

	// for send stream
	NetBase::OutputMemoryStreamPtr pOutputStream;
	auto ret_signal = m_current_state->OnRecvCompleted(pStream,pOutputStream);

#ifdef __DEBUG
	printf("\nsend_id : (%lu)\n", m_newSendID);
#endif // __DEBUG

	if (Send(pOutputStream))
	{
		// ...
	}

	/*--------- data process end ----------*/

	// recv ��������
	if (false == IOCP_Base::IOCPNetworkManager::sInstance->RecvAsync(m_pSock->GetSock(), m_pRecvPacket, shared_from_this()))
	{
		return false;
	}

	return true;
}

bool IOCPSession::OnCompleteSend()
{
	// send packet �� ȸ��
	NetBase::SendPacketPtr pSendPacket = m_sendPacketQueue.front();
	m_sendPacketQueue.pop();
	NetBase::PacketManager::sInstance->RetrieveSendPacket(pSendPacket);

	/*--------- change state process     ----------*/

	// state ȣ��

	/*--------- change state process end ----------*/

	// queue Ȯ��
	// �����ִ� ��Ŷ�� ������� ����
	if (false == m_sendPacketQueue.empty())
	{
		// ��� ����
		if (false == IOCP_Base::IOCPNetworkManager::sInstance->SendAsync(
			m_pSock->GetSock(),
			m_sendPacketQueue.front(),
			shared_from_this()))
			return false;
	}

	return true;
}

void IOCPSession::OnBeforeDisconnected()
{
	// Session ����
}

bool IOCPSession::IsSigned() const
{
	return m_isSigned;
}
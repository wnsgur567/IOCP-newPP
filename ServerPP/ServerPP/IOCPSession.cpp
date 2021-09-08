#include "base.h"


IOCPSessionBasePtr IOCPSession::CreateSession()
{
	IOCPSessionBasePtr retPointer;
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
	if (false == NetIOCP::IOCPNetworkManager::sInstance->RecvAsync(
		m_pSock->GetSock(),
		m_pRecvPacket,
		shared_from_this()))
		return false;

	return true;
}

bool IOCPSession::Send(OutputMemoryStreamPtr pStream)
{
	auto pSendPacket = PacketManager::sInstance->GetSendPacketFromPool();

	//
	pSendPacket->Packing(m_newSendID, pStream);
	++m_newSendID;

	// send queue �� �߰�
	m_sendPacketQueue.push(pSendPacket);

	// ���� �������� send �۾��� ���ٸ� (�ִٸ� ������)
	if (m_sendPacketQueue.size() == 1)
	{	// ��� ����
		if (false == NetIOCP::IOCPNetworkManager::sInstance->SendAsync(
			m_pSock->GetSock(),
			m_sendPacketQueue.front(),
			shared_from_this()))
			return false;
	}

	return true;
}

bool IOCPSession::OnCompleteRecv()
{
	PacketBase::packetId_t packet_id;
	InputMemoryStreamPtr pStream;

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
	OutputMemoryStreamPtr pOutputStream;
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
	if (false == NetIOCP::IOCPNetworkManager::sInstance->RecvAsync(m_pSock->GetSock(), m_pRecvPacket, shared_from_this()))
	{
		return false;
	}

	return true;
}

bool IOCPSession::OnCompleteSend()
{
	// send packet �� ȸ��
	SendPacketPtr pSendPacket = m_sendPacketQueue.front();
	m_sendPacketQueue.pop();
	PacketManager::sInstance->RetrieveSendPacket(pSendPacket);

	/*--------- change state process     ----------*/

	// state ȣ��

	/*--------- change state process end ----------*/

	// queue Ȯ��
	// �����ִ� ��Ŷ�� ������� ����
	if (false == m_sendPacketQueue.empty())
	{
		// ��� ����
		if (false == NetIOCP::IOCPNetworkManager::sInstance->SendAsync(
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
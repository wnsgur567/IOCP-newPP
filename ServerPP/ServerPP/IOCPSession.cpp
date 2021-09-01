#include "base.h"


void IOCPSession::Initialze()
{
	// ù ������ �α��� ���·� ������
	auto pSignState = SignState::Create();
	m_current_state = pSignState;
	m_state_map.insert({ ClientState::EState::Sign,pSignState });
}

IOCPSession::IOCPSession()
	: IOCPSessionBase(),
	m_isSigned(false)
{

}

bool IOCPSession::Recv()
{
	if (false == IOCPNetworkManager::sInstance->RecvAsync(
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
		if (false == IOCPNetworkManager::sInstance->SendAsync(
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
	OutputMemoryStreamPtr pInuputStream = m_current_state->OnRecvCompleted(pStream);

#ifdef __DEBUG
	printf("\nsend_id : (%lu)\n", m_newSendID);
#endif // __DEBUG

	if (Send(pInuputStream))
	{
		// ...
	}

	/*--------- data process end ----------*/

	// recv ��������
	if (false == IOCPNetworkManager::sInstance->RecvAsync(m_pSock->GetSock(), m_pRecvPacket, shared_from_this()))
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
		if (false == IOCPNetworkManager::sInstance->SendAsync(
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
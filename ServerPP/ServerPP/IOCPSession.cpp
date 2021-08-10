#include "base.h"


void IOCPSession::Initialze()
{	

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

	// �Ϸ�� ��Ŷ�� id�� Ȯ��	
	printf("recv_id : (%d, %d)\n", packet_id, m_newRecvID);
	if (packet_id < m_newRecvID)
	{	// �̹� ó�� �Ϸ�� �ߺ� ��Ŷ ���
		return true;
	}

	++m_newRecvID;

	/*--------- data process     ----------*/

	switch (m_state)
	{
	case EState::None:
		break;
	case EState::Sign:
	{
		// ...
		printf("�׽�Ʈ ����\n");		
		char msg[512];
		
		int msg_length;
		pStream->Read(&msg_length, sizeof(msg_length));
		pStream->Read(&msg, msg_length);
		msg[msg_length + 1] = NULL;

		printf("\n���۵� �޽��� ���� : %d\n", msg_length);
		printf("���۵� �޼��� : %s", msg);

		printf("\nsend_id : (%d)\n", m_newSendID);
		auto pSendStream = PacketManager::sInstance->GetSendStreamFromPool();
			
		pSendStream->Write(&msg_length, sizeof(msg_length));
		pSendStream->Write(msg, msg_length);		

		printf("\n�״�� �ٽ� ����...\n");
		if (Send(pSendStream))
		{
			// ...
		}

	}
	break;
	case EState::Disconnected:
		break;
	default:
		break;
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
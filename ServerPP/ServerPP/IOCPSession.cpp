#include "base.h"


void IOCPSession::Initialze()
{
	m_state = EState::Sign;

}

IOCPSession::IOCPSession()
	: IOCPSessionBase(),
	m_state(EState::None), m_isSigned(false)
{
}

IOCPSession::EState IOCPSession::GetState()
{
	return m_state;
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

bool IOCPSession::Send(SendPacketPtr pSendPacket)
{
	// packet id ����
	pSendPacket->SetId(m_newSendID++);

	// send �� stream ��ȣȭ
	pSendPacket->Encryption();

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
	// �Ϸ�� ��Ŷ�� id�� Ȯ��
	auto packet_id = m_pRecvPacket->GetId();
	printf("recv_id : (%d, %d)\n", packet_id, m_newRecvID);
	if (m_pRecvPacket->GetId() < m_newRecvID)
	{	// �̹� ó�� �Ϸ�� �ߺ� ��Ŷ ���
		return true;
	}

	++m_newRecvID;
	// stream ��ȣȭ
	m_pRecvPacket->Decryption();

	/*--------- data process     ----------*/

	switch (m_state)
	{
	case EState::None:
		break;
	case EState::Sign:
	{
		// ...
		printf("�׽�Ʈ ����\n");

		auto pStream = m_pRecvPacket->GetStream();
		char msg[512];
		
		int msg_length;
		pStream->Read(&msg_length, sizeof(msg_length));
		pStream->Read(&msg, msg_length);
		msg[msg_length + 1] = NULL;

		printf("\n���۵� �޽��� ���� : %d\n", msg_length);
		printf("���۵� �޼��� : %s", msg);

		auto pSendPacket = PacketManager::sInstance->GetSendPacketFromPool();
		printf("\nsend_id : (%d)\n", m_newSendID);
		pSendPacket->SetId(m_newSendID++);

		auto pSendStream = pSendPacket->GetStreamPtr();
		pSendStream->Write(&msg_length, sizeof(msg_length));
		pSendStream->Write(msg, msg_length);

		pSendStream->Encryption();

		printf("\n�״�� �ٽ� ����...\n");
		if (Send(pSendPacket))
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


	return false;
}

bool IOCPSession::IsSigned() const
{
	return m_isSigned;
}
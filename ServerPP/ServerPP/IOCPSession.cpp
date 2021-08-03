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
		m_pSock,
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
			m_pSock,
			m_sendPacketQueue.front(),
			shared_from_this()))
			return false;
	}

	return true;
}

bool IOCPSession::OnCompleteRecv()
{
	// �Ϸ�� ��Ŷ�� id�� Ȯ��
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

	}
	break;
	case EState::Disconnected:
		break;
	default:
		break;
	}

	/*--------- data process end ----------*/

	// recv ��������
	if (false == IOCPNetworkManager::sInstance->RecvAsync(m_pSock, m_pRecvPacket, shared_from_this()))
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
#include "IOCPNet_RootHeader.h"


IOCP_Base::IOCPSessionBasePtr IOCPSession::CreateSession()
{
	IOCP_Base::IOCPSessionBasePtr retPointer;
	retPointer.reset(new IOCPSession());
	retPointer->Initialze();
	return retPointer;
}

void IOCPSession::ChangeState(ClientStatePtr pNextState)
{
	m_current_state = pNextState;

	NetBase::OutputMemoryStreamPtr pOutputStream;
	m_current_state->OnChangedToThis(pOutputStream);

	if (pOutputStream == nullptr)
		return;

	if (Send(pOutputStream))
	{
		// ...
	}

}

void IOCPSession::Initialze()
{
	// TODO : ��� state �Ҵ�
	m_sign_state = SignState::Create(shared_from_this());
	m_characterSelect_state = CharacterSelectState::Create(shared_from_this());
	m_village_state = VillageState::Create(shared_from_this());
	m_dungeoun_state = DungeonState::Create(shared_from_this());

	// ù ������ �α��� ���·� ������
	m_current_state = m_sign_state;
}

IOCPSession::IOCPSession()
	: IOCPSessionBase(), m_isSigned(false), m_session_id(0), m_user_id(0), m_player(nullptr)
{

}

bool IOCPSession::Recv()
{

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
	m_current_state->OnRecvCompleted(pStream, pOutputStream);

#ifdef __DEBUG
	printf("\nsend_id : (%lu)\n", m_newSendID);
#endif // __DEBUG

	if (Send(pOutputStream))
	{
		// ...
	}

	/*--------- data process end ----------*/
	m_pRecvPacket->Clear();
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
	m_current_state->OnSendCompleted();

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
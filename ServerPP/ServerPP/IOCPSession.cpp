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
	m_current_state->OnChangedToThis();
}

void IOCPSession::Initialze()
{
	m_player = NetGameObjectManager::sInstance->Create<PlayerInfo>();
	m_player->SetSession(shared_from_this());

	// TODO : 모든 state 할당
	m_sign_state = SignState::Create(shared_from_this());
	m_characterSelect_state = CharacterSelectState::Create(shared_from_this());
	m_village_state = VillageState::Create(shared_from_this());
	m_dungeoun_state = DungeonState::Create(shared_from_this());

	// 첫 시작을 로그인 상태로 변경함
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
	// critical_section
	MyBase::AutoLocker locker(&m_cs);

	// stream 회수
	auto pSendPacket = NetBase::PacketManager::sInstance->GetSendPacketFromPool();

	//
	pSendPacket->Packing(m_newSendID, pStream);
	++m_newSendID;

	NetBase::PacketManager::sInstance->RetrieveSendStream(pStream);

	// send queue 에 추가
	m_sendPacketQueue.push(pSendPacket);

	// 현재 진행중인 send 작업이 없다면 (있다면 보류됨)
	if (m_sendPacketQueue.size() == 1)
	{	// 즉시 전송
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
	// 완료된 패킷의 id를 확인	
	printf("recv_id : (%lu, %lu)\n", packet_id, m_newRecvID);
#endif // __DEBUG

	if (packet_id < m_newRecvID)
	{	// 이미 처리 완료된 중복 패킷 폐기
		return true;
	}

	++m_newRecvID;

	/*--------- data process     ----------*/	
	m_current_state->OnRecvCompleted(pStream);
	/*--------- data process end ----------*/

	m_pRecvPacket->Clear();
	// recv 날려놓기
	if (false == IOCP_Base::IOCPNetworkManager::sInstance->RecvAsync(m_pSock->GetSock(), m_pRecvPacket, shared_from_this()))
	{
		return false;
	}

	return true;
}

bool IOCPSession::OnCompleteSend()
{
	// critical_section
	MyBase::AutoLocker locker(&m_cs);

	// send packet 을 회수
	NetBase::SendPacketPtr pSendPacket = m_sendPacketQueue.front();
	m_sendPacketQueue.pop();
	NetBase::PacketManager::sInstance->RetrieveSendPacket(pSendPacket);

	// queue 확인
	// 남아있는 패킷이 있을경우 전송
	if (false == m_sendPacketQueue.empty())
	{
		// 즉시 전송
		if (false == IOCP_Base::IOCPNetworkManager::sInstance->SendAsync(
			m_pSock->GetSock(),
			m_sendPacketQueue.front(),
			shared_from_this()))
			return false;
	}

	/*--------- change state process     ----------*/
	// state 호출	
	m_current_state->OnSendCompleted();

	/*--------- change state process end ----------*/

	return true;
}

void IOCPSession::OnBeforeDisconnected()
{
	// Session 정리
}
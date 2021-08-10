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

	// send queue 에 추가
	m_sendPacketQueue.push(pSendPacket);

	// 현재 진행중인 send 작업이 없다면 (있다면 보류됨)
	if (m_sendPacketQueue.size() == 1)
	{	// 즉시 전송
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

	// 완료된 패킷의 id를 확인	
	printf("recv_id : (%d, %d)\n", packet_id, m_newRecvID);
	if (packet_id < m_newRecvID)
	{	// 이미 처리 완료된 중복 패킷 폐기
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
		printf("테스트 시작\n");		
		char msg[512];
		
		int msg_length;
		pStream->Read(&msg_length, sizeof(msg_length));
		pStream->Read(&msg, msg_length);
		msg[msg_length + 1] = NULL;

		printf("\n전송된 메시지 길이 : %d\n", msg_length);
		printf("전송된 메세지 : %s", msg);

		printf("\nsend_id : (%d)\n", m_newSendID);
		auto pSendStream = PacketManager::sInstance->GetSendStreamFromPool();
			
		pSendStream->Write(&msg_length, sizeof(msg_length));
		pSendStream->Write(msg, msg_length);		

		printf("\n그대로 다시 전송...\n");
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

	// recv 날려놓기
	if (false == IOCPNetworkManager::sInstance->RecvAsync(m_pSock->GetSock(), m_pRecvPacket, shared_from_this()))
	{
		return false;
	}

	return true;
}

bool IOCPSession::OnCompleteSend()
{
	// send packet 을 회수
	SendPacketPtr pSendPacket = m_sendPacketQueue.front();
	m_sendPacketQueue.pop();
	PacketManager::sInstance->RetrieveSendPacket(pSendPacket);

	/*--------- change state process     ----------*/

	/*--------- change state process end ----------*/

	// queue 확인
	// 남아있는 패킷이 있을경우 전송
	if (false == m_sendPacketQueue.empty())
	{
		// 즉시 전송
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
	// Session 정리
}

bool IOCPSession::IsSigned() const
{
	return m_isSigned;
}
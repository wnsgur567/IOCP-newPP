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
	// packet id 셋팅
	pSendPacket->SetId(m_newSendID++);

	// send 전 stream 암호화
	pSendPacket->Encryption();

	// send queue 에 추가
	m_sendPacketQueue.push(pSendPacket);

	// 현재 진행중인 send 작업이 없다면 (있다면 보류됨)
	if (m_sendPacketQueue.size() == 1)
	{	// 즉시 전송
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
	// 완료된 패킷의 id를 확인
	if (m_pRecvPacket->GetId() < m_newRecvID)
	{	// 이미 처리 완료된 중복 패킷 폐기
		return true;
	}

	++m_newRecvID;
	// stream 복호화
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

	// recv 날려놓기
	if (false == IOCPNetworkManager::sInstance->RecvAsync(m_pSock, m_pRecvPacket, shared_from_this()))
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


	return false;
}

bool IOCPSession::IsSigned() const
{
	return m_isSigned;
}
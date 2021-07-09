#include "base.h"

std::unique_ptr<PacketManager> PacketManager::sInstance;

bool PacketManager::Init(psize_t inStreamPoolingCapacity)
{
	for (psize_t i = 0; i < inStreamPoolingCapacity; i++)
	{
		// acceptpacket
		AcceptPacketPtr a_ptr = std::make_shared<AcceptPacket>();
		a_ptr->Init(a_ptr);
		m_acceptpacket_pool.push(a_ptr);
		m_acceptpacket_container.push_back(a_ptr);
		// recvpacket
		RecvPacketPtr r_ptr = std::make_shared<RecvPacket>(BUFSIZE);
		r_ptr->Init(r_ptr);
		m_recvpacket_pool.push(r_ptr);
		m_recvpacket_container.push_back(r_ptr);
		// sendpacket
		SendPacketPtr s_ptr = std::make_shared<SendPacket>(BUFSIZE);
		s_ptr->Init(s_ptr);
		m_sendpacket_pool.push(s_ptr);
		m_sendpacket_container.push_back(s_ptr);
	}
	return true;
}

bool PacketManager::StaticInit()
{
	sInstance.reset(new PacketManager());

	// stream 을 capacity 만큼 생성 및 풀링
	return PacketManager::sInstance->Init(STREAMPOOLCAPACITY);
}

PacketManager::~PacketManager()
{

}

AcceptPacketPtr PacketManager::GetAcceptPacketFromPool()
{
	AcceptPacketPtr pRetPacket =  m_acceptpacket_pool.front();
	m_acceptpacket_pool.pop();
	pRetPacket->Clear();
	return pRetPacket;
}

void PacketManager::RetrieveAcceptPacket(AcceptPacketPtr inpPacket)
{
	m_acceptpacket_pool.push(inpPacket);
}

RecvPacketPtr PacketManager::GetRecvPacketFromPool()
{
	RecvPacketPtr pRetPacket = m_recvpacket_pool.front();
	m_recvpacket_pool.pop();
	pRetPacket->Clear();
	return pRetPacket;
}

void PacketManager::RetrieveRecvPacket(RecvPacketPtr inpPacket)
{
	m_recvpacket_pool.push(inpPacket);
}

SendPacketPtr PacketManager::GetSendPacketFromPool()
{
	SendPacketPtr pRetPacket = m_sendpacket_pool.front();
	m_sendpacket_pool.pop();
	pRetPacket->Clear();
	return pRetPacket;
}

void PacketManager::RetrieveSendPacket(SendPacketPtr inpPacket)
{
}

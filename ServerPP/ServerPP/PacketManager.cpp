#include "base.h"

Implementation_sInstance(PacketManager);

bool PacketManager::Initialize(LPVOID)
{
	for (psize_t i = 0; i < m_capacity; i++)
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

void PacketManager::Finalize()
{
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
	inpPacket->Clear();
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
	inpPacket->Clear();
	m_sendpacket_pool.push(inpPacket);
}

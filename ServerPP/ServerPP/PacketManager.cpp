#include "base.h"

Implementation_sInstance(PacketManager);

bool PacketManager::Initialize(LPVOID inArgs)
{
	InitializeArgs* pArgs = (InitializeArgs*)inArgs;
	
	for (size_t i = 0; i < pArgs->numberOfAcptPacket; i++)
	{	// acceptpacket
		AcceptPacketPtr a_ptr = std::make_shared<AcceptPacket>();
		a_ptr->Initialize(a_ptr);
		m_acceptpacket_pool.push(a_ptr);
		m_acceptpacket_container.push_back(a_ptr);
	}
	for (size_t i = 0; i < pArgs->numberOfRecvPacket; i++)
	{	// recvpacket
		RecvPacketPtr r_ptr = std::make_shared<RecvPacket>(pArgs->capacityOfRecvBuffer);
		r_ptr->Initialize(r_ptr);
		m_recvpacket_pool.push(r_ptr);
		m_recvpacket_container.push_back(r_ptr);
	}
	for (size_t i = 0; i < pArgs->numberOfSendPacket; i++)
	{	// sendpacket
		SendPacketPtr s_ptr = std::make_shared<SendPacket>(pArgs->capacityOfSendBuffer);
		s_ptr->Initialize(s_ptr);
		m_sendpacket_pool.push(s_ptr);
		m_sendpacket_container.push_back(s_ptr);
	}
	return true;
}

void PacketManager::Finalize()
{
	m_acceptpacket_container.clear();
	while (false == m_acceptpacket_pool.empty())
	{
		m_acceptpacket_pool.pop();
	}

	m_recvpacket_container.clear();
	while (false == m_recvpacket_pool.empty())
	{
		m_recvpacket_pool.pop();
	}

	m_sendpacket_container.clear();
	while (false == m_sendpacket_pool.empty())
	{
		m_sendpacket_pool.pop();
	}
	
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

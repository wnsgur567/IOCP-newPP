#include "NetBase_RootHeader.h"

Implementation_sInstance(NetBase::PacketManager);

namespace NetBase
{

	bool PacketManager::Initialize(LPVOID inArgs) noexcept
	{
		InitializeArgs* pArgs = (InitializeArgs*)inArgs;

		for (size_t i = 0; i < pArgs->numberOfAcptPacket; i++)
		{	// acceptpacket
			AcceptPacketPtr a_ptr = AcceptPacket::Create();
			m_acceptpacket_pool.push(a_ptr);
			m_acceptpacket_container.push_back(a_ptr);
		}
		for (size_t i = 0; i < pArgs->numberOfRecvPacket; i++)
		{	// recvpacket
			RecvPacketPtr r_ptr = RecvPacket::Create(pArgs->capacityOfRecvBuffer);
			m_recvpacket_pool.push(r_ptr);
			m_recvpacket_container.push_back(r_ptr);
		}
		for (size_t i = 0; i < pArgs->numberOfSendPacket; i++)
		{	// sendpacket
			SendPacketPtr s_ptr = SendPacket::Create(pArgs->capacityOfSendBuffer);
			m_sendpacket_pool.push(s_ptr);
			m_sendpacket_container.push_back(s_ptr);

			// stream
			OutputMemoryStreamPtr pStream = std::make_shared<OutputMemoryStream>(pArgs->capacityOfSendBuffer);
			m_sendStream_queue.push(pStream);
			m_sendStream_container.push_back(pStream);
		}
		return true;
	}

	void PacketManager::Finalize() noexcept
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
		AcceptPacketPtr pRetPacket = m_acceptpacket_pool.front();
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

		auto pStream = inpPacket->m_pStream;
		pStream->Clear();
		m_sendStream_queue.push(pStream);
	}

	OutputMemoryStreamPtr PacketManager::GetSendStreamFromPool()
	{
		OutputMemoryStreamPtr pStream = m_sendStream_queue.front();
		m_sendStream_queue.pop();
		return pStream;
	}
}
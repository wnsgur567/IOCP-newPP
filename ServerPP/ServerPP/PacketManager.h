#pragma once

// Initialize 시 내부에 정의한 Args 구조체를 정의하여 넘겨줄 것
class PacketManager : public Singleton<PacketManager>
{
private:
	friend class Singleton;
	friend class IOCPNetworkManager;
public:
	using packetSize_t = PacketBase::packetSize_t;
	using packetId_t = ::PacketBase::packetId_t;
private:
	PacketManager()
		: m_recvpacket_pool(), m_sendpacket_pool()
	{}
public:
	~PacketManager();

	struct InitializeArgs
	{
		size_t numberOfAcptPacket;		// Pooling 할 AcceptPackt 개수
		size_t numberOfRecvPacket;		// Pooling 할 RecvPacket 개수
		size_t capacityOfRecvBuffer;	// RecvPacket 의 buffer 최대 용량
		size_t numberOfSendPacket;		// Pooling 할 SendPacket 개수
		size_t capacityOfSendBuffer;	// SendPacket 의 buffer 최대 용량
	};

	bool Initialize(LPVOID) override;
	void Finalize() override;
private:

	// 풀링한 패킷이 사라지지 않도록 hold 할 보관 컨테이너임...
	std::list<AcceptPacketPtr> m_acceptpacket_container;
	std::list<RecvPacketPtr> m_recvpacket_container;
	std::list<SendPacketPtr> m_sendpacket_container;

	// iocp를 위한 스트림 풀링
	// 일반 동기화의 경우 소켓당 한개씩만 매칭 시킬 것
	std::queue<AcceptPacketPtr> m_acceptpacket_pool;
	std::queue<RecvPacketPtr> m_recvpacket_pool;
	std::queue<SendPacketPtr> m_sendpacket_pool;

public:
	AcceptPacketPtr GetAcceptPacketFromPool();
	void RetrieveAcceptPacket(AcceptPacketPtr inpPacket);
	RecvPacketPtr GetRecvPacketFromPool();
	void RetrieveRecvPacket(RecvPacketPtr inpPacket);
	SendPacketPtr GetSendPacketFromPool();
	void RetrieveSendPacket(SendPacketPtr inpPacket);
};
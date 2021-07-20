#pragma once

class PacketManager : public Singleton<PacketManager>
{
	friend class Singleton;
	friend class IOCPNetworkManager;
public:
	using psize_t = unsigned __int32;
	using id_t = unsigned __int32;

private:
	PacketManager()
		: m_capacity(STREAMPOOLCAPACITY),
		m_recvpacket_pool(), m_sendpacket_pool()
	{}
public:
	~PacketManager();

	bool Initialize(LPVOID) override;
	void Finalize() override;
private:
	psize_t m_capacity;

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

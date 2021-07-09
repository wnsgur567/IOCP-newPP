#pragma once

class PacketManager
{
	friend class IOCPNetworkManager;
public:
	static std::unique_ptr<PacketManager> sInstance;
	using psize_t = unsigned __int32;
	using id_t = unsigned __int32;	

private:
	PacketManager()
		:m_curRecvPacketID(0), m_newSendPacketID(0),
		m_recvpacket_pool(), m_sendpacket_pool()		
	{}
	bool Init(psize_t inStreamPoolingCapacity);
public:
	PacketManager(const PacketManager&) = delete;
	PacketManager& operator=(const PacketManager&) = delete;
	static bool StaticInit();
	~PacketManager();

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
	
private:
	// TODO : 각 클라이언트 별로 관리하게 바꾸자
	unsigned __int32 m_curRecvPacketID;		// 최근에 받은 recv packet id
	unsigned __int32 m_newSendPacketID;		// 새로 만들 send packet id
public:
	AcceptPacketPtr GetAcceptPacketFromPool();
	void RetrieveAcceptPacket(AcceptPacketPtr inpPacket);
	RecvPacketPtr GetRecvPacketFromPool();
	void RetrieveRecvPacket(RecvPacketPtr inpPacket);
	SendPacketPtr GetSendPacketFromPool();
	void RetrieveSendPacket(SendPacketPtr inpPacket);
};

// session 에서 씀
//SendPacketPtr Packing(const void* inData, const size_t inSize);	// 받은 data 에 패킷id 와 size를 부여해서 패킹
//void UnPacking(const RecvPacketPtr inPacket, void* outData, size_t* outSize);
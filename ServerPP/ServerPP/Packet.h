#pragma once

class PacketBase;
using PacketBasePtr = std::shared_ptr<PacketBase>;
using PacketBaseWeakPtr = std::weak_ptr<PacketBase>;

struct OverlappedEx
{
	enum class EOverlappedType
	{
		Accept,
		Recv,
		Send,
	};

	WSAOVERLAPPED overlapped;
	EOverlappedType type;
	PacketBaseWeakPtr pPacket;
	std::shared_ptr<void> pointer;	// 임의의 포인터를 담을 포인터

	OverlappedEx(EOverlappedType inType)
		: overlapped(), type(inType)
		, pointer(nullptr)
	{

	}
	void Initialize(PacketBasePtr inpPacket)
	{
		pPacket = inpPacket;
	}
	void flush()
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
	}
};

class PacketBase
{
public:
	using packetSize_t = ::gsize_t;
	using packetId_t = ::gid_t;

	enum class EPacketState
	{
		Error = -3,			// 비정상 종료
		End = -2,			// 정상 종료
		Duplicated = -1,	// 중복 패킷

		Idle = 0,
		InComplete,		// 미완성, 데이터 받는(전송) 중
		Completed,		// 완성
	};

protected:
	packetId_t		m_id;
	EPacketState	m_state;
	OverlappedEx	m_overlappedEx;
public:
	unsigned __int32 GetId() const { return m_id; }
	void SetId(unsigned __int32 inId) { m_id = inId; }

	virtual void Initialize(PacketBasePtr) = 0;
	virtual void Clear() = 0;
	OverlappedEx& GetOverlappedRef() { return m_overlappedEx; }
protected:
	PacketBase(OverlappedEx::EOverlappedType inType) :
		m_id(0), m_state(EPacketState::Idle),
		m_overlappedEx(inType)
	{

	}
	virtual ~PacketBase()
	{

	}
};

class AcceptPacket;
using AcceptPacketPtr = std::shared_ptr<AcceptPacket>;

class AcceptPacket : public PacketBase
{
	friend class PacketManager;
	template <typename T>
	friend class IOCPNetworkManagerBase;
protected:
	TCPSocketPtr m_pClientSock;
	SocketAddress m_sockAddr;

public:
	AcceptPacket()
		:PacketBase(OverlappedEx::EOverlappedType::Accept)
	{

	}

	void Initialize(PacketBasePtr inpPacket) override;
	void GetReady();

	void Set(TCPSocketPtr inpSock, SocketAddress inAddr);
	TCPSocketPtr GetPSock();
	SocketAddress GetAddr();
	void Clear() override;
};

class RecvPacket;
using RecvPacketPtr = std::shared_ptr<RecvPacket>;
// using input stream
class RecvPacket : public PacketBase
{
	friend class PacketManager;
	template <typename T>
	friend class IOCPNetworkManagerBase;
public:
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

private:
	IOCPInputMemoryStreamPtr m_pStream;	// session 에서 사용할 datastream

	WSABUF			m_wsabuf;
	bool			m_sizeflag;
	packetSize_t	m_recvbytes;
	packetSize_t	m_target_recvbytes;

	time_point_t m_recv_time;	// 패킷 수신이 완료된 시간 complete recv
public:
	RecvPacket(packetSize_t inStreamCapacity);
	~RecvPacket();

	// packet을 새로 할당 시 정보 초기화용
	void Initialize(PacketBasePtr inpThis);
	// packet을 Pool에서 가져올때 정보 초기화용
	void Clear() override;

	// recv 전 overlapped 및 wsabuf 초기화
	void GetReady();
	IOCPInputMemoryStreamPtr GetStream();		

	// 패킷 recv 가 완료된 시간을 기록
	void RecordRecvTime();
	time_point_t GetRecvTime() const;
};

class SendPacket;
using SendPacketPtr = std::shared_ptr<SendPacket>;
// using output stream
class SendPacket : public PacketBase
{
	friend class PacketManager;
	template <typename T>
	friend class IOCPNetworkManagerBase;
private:
	IOCPOutputMemoryStreamPtr m_pStream; // session 에서 사용할 datastream

	WSABUF				m_wsabuf;
	packetSize_t		m_sendbytes;				// 현재 send 수치
	packetSize_t		m_target_sendbytes;			// 목표 send 수치
public:
	SendPacket(packetSize_t inStreamCapacity);

	// packet을 새로 할당 시 정보 초기화용
	void Initialize(PacketBasePtr inpThis) override;
	// packet을 Pool에서 가져올때 정보 초기화용
	void Clear() override;

	// send 전 overlapped 및 wsabuf 초기화 && packgin
	void GetReady(const packetId_t inPacketID);	
};
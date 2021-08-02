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
	std::shared_ptr<void> pointer;	// ������ �����͸� ���� ������

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
		Error = -3,			// ������ ����
		End = -2,			// ���� ����
		Duplicated = -1,	// �ߺ� ��Ŷ

		Idle = 0,
		InComplete,		// �̿ϼ�, ������ �޴�(����) ��
		Completed,		// �ϼ�
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
	IOCPInputMemoryStreamPtr m_pStream;	// session ���� ����� datastream

	WSABUF			m_wsabuf;
	bool			m_sizeflag;
	packetSize_t	m_recvbytes;
	packetSize_t	m_target_recvbytes;

	time_point_t m_recv_time;	// ��Ŷ ������ �Ϸ�� �ð� complete recv
public:
	RecvPacket(packetSize_t inStreamCapacity);
	~RecvPacket();

	// packet�� ���� �Ҵ� �� ���� �ʱ�ȭ��
	void Initialize(PacketBasePtr inpThis);
	// packet�� Pool���� �����ö� ���� �ʱ�ȭ��
	void Clear() override;

	// recv �� overlapped �� wsabuf �ʱ�ȭ
	void GetReady();
	IOCPInputMemoryStreamPtr GetStream();		

	// ��Ŷ recv �� �Ϸ�� �ð��� ���
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
	IOCPOutputMemoryStreamPtr m_pStream; // session ���� ����� datastream

	WSABUF				m_wsabuf;
	packetSize_t		m_sendbytes;				// ���� send ��ġ
	packetSize_t		m_target_sendbytes;			// ��ǥ send ��ġ
public:
	SendPacket(packetSize_t inStreamCapacity);

	// packet�� ���� �Ҵ� �� ���� �ʱ�ȭ��
	void Initialize(PacketBasePtr inpThis) override;
	// packet�� Pool���� �����ö� ���� �ʱ�ȭ��
	void Clear() override;

	// send �� overlapped �� wsabuf �ʱ�ȭ && packgin
	void GetReady(const packetId_t inPacketID);	
};
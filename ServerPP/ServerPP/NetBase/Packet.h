#pragma once

// shared from this Ȥ�� weak from this�� �����ڿ��� ȣ���ϸ� �ȵ�
// ��?		 : shared ptr ���� �������� �ش� �����ڰ� �������� �����ǹǷ�
// �ذ� ���  : �����ڴ� ���� ���ϵ��� ��װ� Crerate �Լ��� ������ �����ϰ� ���ο��� �ʱ�ȭ �Ұ�
// ���  	 : shared from this �� ����Լ����� ��� �� ��쿡 �ʿ���

namespace NetBase
{
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
		VoidPtr pointer;	// ������ �����͸� ���� ������

		OverlappedEx(EOverlappedType inType)
			: overlapped(), type(inType) /*, pPacket()*/
			, pointer(nullptr)
		{

		}
		void flush()
		{
			ZeroMemory(&overlapped, sizeof(overlapped));
		}
	};

	class PacketBase
	{
	public:
		using packetSize_t = ::gid32_t;
		using packetId_t = ::gid32_t;

		enum class EPacketState
		{
			Error = -3,			// ������ ����
			End = -2,			// ���� ����
			Duplicated = -1,	// �ߺ� ��Ŷ

			Idle = 0,
			InComplete,		// �̿ϼ�, ������ �޴�(����) ��
			Completed,		// �ϼ�
		};

	public:
		packetId_t		m_id;
		EPacketState	m_state;
		OverlappedEx	m_overlappedEx;
	public:
		virtual void Clear() = 0;
		OverlappedEx* GetOverlappedPtr() { return &m_overlappedEx; }
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

	class AcceptPacket
		: public PacketBase
	{
	public:
		TCPSocketPtr m_pClientSock;
		SocketAddress m_sockAddr;
	private:
		AcceptPacket()
			:PacketBase(OverlappedEx::EOverlappedType::Accept)
		{
		}
	public:
		static AcceptPacketPtr Create();
	public:
		void GetReady();

		void Set(TCPSocketPtr inpSock, SocketAddress inAddr);
		TCPSocketPtr GetPSock();
		SocketAddress GetAddr();
		void Clear() override;
	};

	class RecvPacket;
	using RecvPacketPtr = std::shared_ptr<RecvPacket>;
	// using input stream
	class RecvPacket
		: public PacketBase
	{
	public:
		using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

	public:
		InputMemoryStreamPtr m_pStream;	// session ���� ����� datastream

		WSABUF			m_wsabuf;
		bool			m_sizeflag;
		packetSize_t	m_recvbytes;
		packetSize_t	m_target_recvbytes;

		time_point_t m_recv_time;	// ��Ŷ ������ �Ϸ�� �ð� complete recv
	private:
		RecvPacket();
	public:
		// packet�� Pool���� �����ö� ���� �ʱ�ȭ��
		void Clear() override;
		// recv �� overlapped �� wsabuf �ʱ�ȭ
		void GetReady();

		// ��Ŷ recv �� �Ϸ�� �ð��� ���
		void RecordRecvTime();
		time_point_t GetRecvTime() const;
	public:
		static RecvPacketPtr Create(packetSize_t inStreamCapacity);
		void UnPackging(packetId_t& outID, InputMemoryStreamPtr& outpStream);
	};

	class SendPacket;
	using SendPacketPtr = std::shared_ptr<SendPacket>;
	// using output stream
	class SendPacket
		: public PacketBase
	{
	public:
		OutputMemoryStreamPtr m_pStream; // session ���� ����� datastream

		WSABUF				m_wsabuf;
		packetSize_t		m_sendbytes;				// ���� send ��ġ
		packetSize_t		m_target_sendbytes;			// ��ǥ send ��ġ
	private:
		SendPacket();
	public:
		// packet�� Pool���� �����ö� ���� �ʱ�ȭ��
		void Clear() override;
		// send �� overlapped �� wsabuf �ʱ�ȭ
		void GetReady();
	public:
		static SendPacketPtr Create(packetSize_t inStreamCapacity);
		void Packing(packetId_t inID, OutputMemoryStreamPtr inStream);
	};

}
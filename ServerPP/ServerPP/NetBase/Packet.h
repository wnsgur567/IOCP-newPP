#pragma once

// shared from this 혹은 weak from this는 생성자에서 호출하면 안됨
// 왜?		 : shared ptr 관련 변수들은 해당 생성자가 끝난이후 생성되므로
// 해결 방안  : 생성자는 접근 못하도록 잠그고 Crerate 함수를 별도로 지정하고 내부에서 초기화 할것
// 결론  	 : shared from this 는 멤버함수에서 사용 할 경우에 필요함

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
		VoidPtr pointer;	// 임의의 포인터를 담을 포인터

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
			Error = -3,			// 비정상 종료
			End = -2,			// 정상 종료
			Duplicated = -1,	// 중복 패킷

			Idle = 0,
			InComplete,		// 미완성, 데이터 받는(전송) 중
			Completed,		// 완성
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
		InputMemoryStreamPtr m_pStream;	// session 에서 사용할 datastream

		WSABUF			m_wsabuf;
		bool			m_sizeflag;
		packetSize_t	m_recvbytes;
		packetSize_t	m_target_recvbytes;

		time_point_t m_recv_time;	// 패킷 수신이 완료된 시간 complete recv
	private:
		RecvPacket();
	public:
		// packet을 Pool에서 가져올때 정보 초기화용
		void Clear() override;
		// recv 전 overlapped 및 wsabuf 초기화
		void GetReady();

		// 패킷 recv 가 완료된 시간을 기록
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
		OutputMemoryStreamPtr m_pStream; // session 에서 사용할 datastream

		WSABUF				m_wsabuf;
		packetSize_t		m_sendbytes;				// 현재 send 수치
		packetSize_t		m_target_sendbytes;			// 목표 send 수치
	private:
		SendPacket();
	public:
		// packet을 Pool에서 가져올때 정보 초기화용
		void Clear() override;
		// send 전 overlapped 및 wsabuf 초기화
		void GetReady();
	public:
		static SendPacketPtr Create(packetSize_t inStreamCapacity);
		void Packing(packetId_t inID, OutputMemoryStreamPtr inStream);
	};

}
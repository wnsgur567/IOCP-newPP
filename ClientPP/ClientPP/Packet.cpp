#include "base.h"

void AcceptPacket::GetReady()
{
	// ... 없음
}

void AcceptPacket::Set(TCPSocketPtr inpSock, SocketAddress inAddr)
{
	m_pClientSock = inpSock;
	m_sockAddr = inAddr;
}

TCPSocketPtr AcceptPacket::GetPSock()
{
	return m_pClientSock;
}

SocketAddress AcceptPacket::GetAddr()
{
	return m_sockAddr;
}

void AcceptPacket::Clear()
{
	m_pClientSock = nullptr;
	ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
	m_overlappedEx.flush();
}


RecvPacket::RecvPacket(packetSize_t inStreamCapacity)
	: PacketBase(OverlappedEx::EOverlappedType::Recv),
	m_pStream(nullptr),
	m_sizeflag(true),
	m_recvbytes(0),
	m_target_recvbytes(0)
{
	m_overlappedEx.pPacket = weak_from_this();
	m_pStream = std::make_shared<IOCPInputMemoryStream>(inStreamCapacity);
}

RecvPacket::~RecvPacket()
{

}

void RecvPacket::GetReady()
{
	BYTE* ptr = m_pStream->m_buffer;

	// overlap 초기화
	m_overlappedEx.flush();

	// wsabuf 초기화
	m_wsabuf.buf = (CHAR*)ptr + m_recvbytes;
	if (m_sizeflag)
	{
		m_wsabuf.len = sizeof(packetSize_t) - m_recvbytes;
	}
	else
	{
		m_wsabuf.len = m_target_recvbytes - m_recvbytes;
	}
}

IOCPInputMemoryStreamPtr RecvPacket::GetStream()
{
	return m_pStream;
}

void RecvPacket::Decryption()
{
	// buffer 에 바로 작업함
	CipherManager::sInstance->Decryption(m_pStream->m_buffer + sizeof(packetId_t) + sizeof(packetSize_t), m_pStream->GetLength());
}

void RecvPacket::RecordRecvTime()
{
	m_recv_time = std::chrono::high_resolution_clock::now();
}

RecvPacket::time_point_t RecvPacket::GetRecvTime() const
{
	return m_recv_time;
}

void RecvPacket::Clear()
{
	// base
	m_id = 0;
	m_state = EPacketState::Idle;

	// this
	m_sizeflag = true;
	m_recvbytes = 0;
	m_target_recvbytes = 0;

	m_overlappedEx.flush();
}

SendPacket::SendPacket(packetSize_t inStreamCapacity)
	:PacketBase(OverlappedEx::EOverlappedType::Send),
	m_pStream(nullptr),
	m_sendbytes(0),
	m_target_sendbytes(0)

{
	m_overlappedEx.pPacket = weak_from_this();
	m_pStream = std::make_shared<IOCPOutputMemoryStream>(inStreamCapacity);
}

void SendPacket::GetReady()
{
	BYTE* buf = m_pStream->m_buffer;

	// 패킷 구성 : total packet size + packet id + stream

	m_overlappedEx.flush();

	if (m_state == EPacketState::Idle)
	{	// 처음 Send 하는 경우

		// total packet size = packet id size + stream size
		packetSize_t total_size = sizeof(packetId_t) + m_pStream->GetLength();
		memcpy(buf, &total_size, sizeof(packetSize_t));
		memcpy(buf + sizeof(packetSize_t), &m_id, sizeof(packetId_t));
		memcpy(buf + sizeof(packetSize_t) + sizeof(packetId_t), m_pStream->GetBufferPtr(), m_pStream->GetLength());
		m_target_sendbytes = total_size + sizeof(packetSize_t);


		// TODO : Encryption
	}

	// wsa buf 초기화
	m_wsabuf.buf = (CHAR*)buf + m_sendbytes;
	m_wsabuf.len = m_target_sendbytes - m_sendbytes;
}

void SendPacket::Encryption()
{
	CipherManager::sInstance->Encryption(m_pStream->m_buffer + sizeof(packetId_t) + sizeof(packetSize_t), m_pStream->GetLength());
}

void SendPacket::Clear()
{
	m_state = EPacketState::Idle;
	m_sendbytes = 0;
	m_target_sendbytes = 0;
	m_overlappedEx.pointer = nullptr;
}

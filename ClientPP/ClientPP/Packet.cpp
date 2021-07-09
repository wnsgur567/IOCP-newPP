#include "base.h"

RecvPacket::RecvPacket(psize_t inStreamCapacity)
	: PacketBase(E_OverlappedType::Recv, inStreamCapacity + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(nullptr),
	m_sizeflag(true),
	m_recvbytes(0),
	m_target_recvbytes(0)
{
	m_pStream = std::make_shared<InputMemoryStream>(inStreamCapacity);
}

RecvPacket::RecvPacket(InputMemoryStreamPtr inStreamPtr)
	: PacketBase(E_OverlappedType::Recv, inStreamPtr->GetCapacity() + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(inStreamPtr),
	m_sizeflag(true),
	m_recvbytes(0),
	m_target_recvbytes(0)
{

}

RecvPacket::~RecvPacket()
{

}

void RecvPacket::GetReady()
{
	// overlap 초기화
	m_overlappedEx.flush();

	// 


	// wsabuf 초기화
	m_wsabuf.buf = m_pStream->m_buffer + m_recvbytes;
	if (m_sizeflag)
	{
		m_wsabuf.len = sizeof(psize_t) - m_recvbytes;
	}
	else
	{
		m_wsabuf.len = m_target_recvbytes - m_recvbytes;
	}
}

InputMemoryStreamPtr RecvPacket::GetStream()
{
	return m_pStream;
}

void RecvPacket::RecordRecvTime()
{
	m_recv_time = std::chrono::high_resolution_clock::now();
}

RecvPacket::time_point_t RecvPacket::GetRecvTime() const
{
	return m_recv_time;
}

void RecvPacket::Init(PacketBasePtr inpThis)
{
	m_overlappedEx.pPacket = inpThis;
}

void RecvPacket::Clear()
{
	// base
	m_id = 0;
	m_state = E_PacketState::Idle;

	// this
	m_sizeflag = true;
	m_recvbytes = 0;
	m_target_recvbytes = 0;
}


SendPacket::SendPacket(psize_t inStreamCapacity)
	:PacketBase(E_OverlappedType::Send, inStreamCapacity + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(nullptr),
	m_sendbytes(0),
	m_target_sendbytes(0)

{
}

SendPacket::SendPacket(OutputMemoryStreamPtr inStreamPtr)
	: PacketBase(E_OverlappedType::Send, inStreamPtr->GetCapacity() + sizeof(psize_t) + sizeof(id_t)),
	m_pStream(inStreamPtr),
	m_sendbytes(0),
	m_target_sendbytes(0)
{

}

void SendPacket::GetReady(const id_t inPacketID)
{
	// 패킷 구성 : total packet size + packet id + stream

	m_overlappedEx.flush();
	if (m_state == E_PacketState::Idle)
	{	// 처음 Send 하는 경우
		// total packet size = packet id size + stream size
		psize_t total_size = sizeof(id_t) + m_pStream->GetLength();
		memcpy(m_buf, &total_size, sizeof(psize_t));
		memcpy(m_buf + sizeof(psize_t), &inPacketID, sizeof(id_t));
		memcpy(m_buf + sizeof(psize_t) + sizeof(id_t), m_pStream->GetBufferPtr(), m_pStream->GetLength());
		m_target_sendbytes = total_size + sizeof(psize_t);
	}

	m_wsabuf.buf = m_buf + m_sendbytes;
	m_wsabuf.len = m_target_sendbytes - m_sendbytes;
}

void SendPacket::Init(PacketBasePtr inpThis)
{
	m_overlappedEx.Init(inpThis);
}

void SendPacket::Clear()
{
	m_state = E_PacketState::Idle;
	m_sendbytes = 0;
	m_target_sendbytes = 0;
}

void AcceptPacket::Init(PacketBasePtr inpPacket)
{
	m_overlappedEx.Init(inpPacket);
}

void AcceptPacket::GetReady()
{
	// ... 없음
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
}

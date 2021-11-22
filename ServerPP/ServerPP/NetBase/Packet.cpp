#include "NetBase_RootHeader.h"
#include "../Utils/KISA_CIPHER/CipherManager.h"



namespace NetBase
{

	AcceptPacketPtr AcceptPacket::Create()
	{
		AcceptPacketPtr pThis(new AcceptPacket());
		pThis->m_overlappedEx.pPacket = pThis;
		return pThis;
	}

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


	RecvPacket::RecvPacket()
		: PacketBase(OverlappedEx::EOverlappedType::Recv),
		m_pStream(nullptr),
		m_wsabuf(),
		m_sizeflag(true),
		m_recvbytes(0),
		m_target_recvbytes(0)
	{

	}

	RecvPacketPtr RecvPacket::Create(packetSize_t inStreamCapacity)
	{
		RecvPacketPtr pThis(new RecvPacket);
		pThis->m_pStream = std::make_shared<InputMemoryStream>(inStreamCapacity);
		pThis->m_overlappedEx.pPacket = pThis;
		return pThis;
	}

	void RecvPacket::GetReady()
	{
		// overlap 초기화
		m_overlappedEx.flush();

		// wsabuf 초기화	
		if (m_sizeflag)
		{
			m_wsabuf.buf = (CHAR*)&m_target_recvbytes + m_recvbytes;
			m_wsabuf.len = sizeof(packetSize_t) - m_recvbytes;
		}
		else
		{
			BYTE* ptr = m_pStream->GetBufferPtr();
			m_wsabuf.buf = (CHAR*)ptr + m_recvbytes;
			m_wsabuf.len = m_target_recvbytes - m_recvbytes;
		}
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

		m_pStream->Clear();
		m_overlappedEx.flush();
	}

	void RecvPacket::UnPackging(packetId_t& outID, InputMemoryStreamPtr& outpStream)
	{
		BYTE* ptr = m_pStream->GetBufferPtr();

		// id
		NetBase::ReadFromBinStream(m_pStream, outID);
		//m_pStream->Read(&outID, sizeof(packetId_t));

#ifdef __CIPHER_ON
		// decryption
		NetCipher::CipherManager::sInstance->Decryption(
			m_pStream->GetBufferPtr() + sizeof(packetId_t),
			m_pStream->GetLength() - sizeof(packetId_t));
#else

#endif

		// stream
		outpStream = m_pStream;
	}

	SendPacket::SendPacket()
		:PacketBase(OverlappedEx::EOverlappedType::Send),
		m_pStream(nullptr),
		m_wsabuf(),
		m_sendbytes(0),
		m_target_sendbytes(0)

	{

	}

	SendPacketPtr SendPacket::Create(packetSize_t inStreamCapacity)
	{
		SendPacketPtr pThis(new SendPacket());
		pThis->m_pStream = std::make_shared<OutputMemoryStream>(inStreamCapacity);
		pThis->m_overlappedEx.pPacket = pThis;
		return pThis;
	}

	void SendPacket::GetReady()
	{
		BYTE* buf = m_pStream->GetBufferPtr();

		m_overlappedEx.flush();

		// wsa buf 초기화
		m_wsabuf.buf = (CHAR*)buf + m_sendbytes;
		m_wsabuf.len = m_target_sendbytes - m_sendbytes;
	}



	void SendPacket::Clear()
	{
		m_state = EPacketState::Idle;
		m_sendbytes = 0;
		m_target_sendbytes = 0;
		m_overlappedEx.pointer = nullptr;
	}

	// total size + packetid + data
	// memory copy from instream to sendpacket's stream
	void SendPacket::Packing(packetId_t id, OutputMemoryStreamPtr inStream)
	{

#ifdef __CIPHER_ON
		// data encryption
		// ret bits 는 패딩 비트가 포함된 크기임
		size_t retBits = NetCipher::CipherManager::sInstance->Encryption(
			inStream->GetBufferPtr(),
			inStream->GetLength());
#else
		size_t retBits = inStream->GetLength();
#endif

		// total size = id size + data size
		packetSize_t total_size = sizeof(packetId_t) + static_cast<packetSize_t>(retBits);
		m_pStream->Write(&total_size, sizeof(packetSize_t));

		// id
		m_pStream->Write(&id, sizeof(packetId_t));

		// data
		m_pStream->Write(inStream->GetBufferPtr(), retBits);

		// bytes setting
		m_target_sendbytes = total_size + sizeof(packetSize_t);
	}


	SQLPacket::SQLPacket()
		: PacketBase(OverlappedEx::EOverlappedType::SQL), query()
	{
	}
	SQLPacketPtr SQLPacket::Create()
	{
		SQLPacketPtr pThis(new SQLPacket());	
		pThis->m_overlappedEx.pPacket = pThis;
		return pThis;
	}
	void SQLPacket::Clear()
	{
		query.clear();
	}
}
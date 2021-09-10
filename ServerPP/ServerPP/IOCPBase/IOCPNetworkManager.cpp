#include "IOCPBase_RootHeader.h"

#include <handleapi.h>

Implementation_sInstance(IOCP_Base::IOCPNetworkManager);

namespace IOCP_Base
{

	bool IOCPNetworkManager::Initialize(LPVOID arg) noexcept
	{
#ifdef  __DEBUG
		printf("\n----- IOCPNetworkManager::Initialize() -----\n");
#endif //  __DEBUG

		// iocp 입출력 포트 생성
		m_pHcp = NetBase::SocketUtil::CreateIOCP(IOCPNetworkManager::WorkerThread, m_hWorkerThreads);
		if (m_pHcp.get() == nullptr)
			return false;

		// parent init
		if (false == NetworkManagerServer::Initialize(arg))
			return false;

#ifdef  __DEBUG
		printf("----- IOCPNetworkManager::Initialize() end -----\n");
#endif //  __DEBUG
		return true;
	}

	void IOCPNetworkManager::Finalize() noexcept
	{
#ifdef  __DEBUG
		printf("\n----- IOCPNetworkManager::Finalize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- IOCPNetworkManager::Finalize() Complete -----\n");
#endif //  __DEBUG
	}


	HandlePtr IOCPNetworkManager::GetHCPPtr() const
	{
		return m_pHcp;
	}

	bool IOCPNetworkManager::RecvAsync(SOCKET inSock, NetBase::RecvPacketPtr inpRecvPacket, VoidPtr inPointer)
	{
		int retval;
		DWORD recvbytes;
		DWORD flags = 0;

		// overlap wsa 준비셋팅
		inpRecvPacket->GetReady();
		inpRecvPacket->m_overlappedEx.pointer = inPointer;

		// 비동기 async 수행
		retval = WSARecv(inSock,
			&inpRecvPacket->m_wsabuf, 1, &recvbytes, &flags,
			&inpRecvPacket->m_overlappedEx.overlapped, nullptr);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				NetBase::SocketUtil::ReportError("PacketManager::RecvAsync()");
				return false;
			}
		}

		return true;
	}

	bool IOCPNetworkManager::SendAsync(SOCKET inSock, NetBase::SendPacketPtr inpSendPacket, VoidPtr inPointer)
	{
		int retval;
		DWORD sendbytes;
		DWORD flags = 0;

		inpSendPacket->GetReady();
		inpSendPacket->m_overlappedEx.pointer = inPointer;

		retval = WSASend(inSock,
			&inpSendPacket->m_wsabuf, 1, &sendbytes,
			0, &inpSendPacket->m_overlappedEx.overlapped, nullptr);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				NetBase::SocketUtil::ReportError("PacketManager::SendAsync()");
				return false;
			}
		}

		return true;
	}

	NetBase::PacketBase::EPacketState IOCPNetworkManager::CompleteRecv(SOCKET inSock, NetBase::RecvPacketPtr inpRecvPacket, VoidPtr inPointer, const packetSize_t inCompletebyte)
	{
		BYTE* ptr = inpRecvPacket->m_pStream->GetBufferPtr();

		if (inpRecvPacket->m_sizeflag)
		{
			inpRecvPacket->m_recvbytes += inCompletebyte;

			if (inpRecvPacket->m_recvbytes == sizeof(NetBase::PacketBase::packetSize_t))
			{
				inpRecvPacket->m_recvbytes = 0;
				inpRecvPacket->m_sizeflag = false;
			}

			if (false == RecvAsync(inSock, inpRecvPacket, inPointer))
			{
				return  NetBase::PacketBase::EPacketState::Error;
			}

			return  NetBase::PacketBase::EPacketState::InComplete;
		}

		inpRecvPacket->m_recvbytes += inCompletebyte;

		if (inpRecvPacket->m_recvbytes != inpRecvPacket->m_target_recvbytes)
		{
			if (false == RecvAsync(inSock, inpRecvPacket, inPointer))
			{
				return  NetBase::PacketBase::EPacketState::Error;
			}
			return  NetBase::PacketBase::EPacketState::InComplete;
		}

		inpRecvPacket->m_pStream->SetLenth(inpRecvPacket->m_recvbytes);

		return  NetBase::PacketBase::EPacketState::Completed;
	}

	NetBase::PacketBase::EPacketState IOCPNetworkManager::CompleteSend(SOCKET inSock, NetBase::SendPacketPtr inpSendPacket, VoidPtr inPointer, const packetSize_t inCompletebyte)
	{
		inpSendPacket->m_sendbytes += inCompletebyte;
		if (inpSendPacket->m_sendbytes != inpSendPacket->m_target_sendbytes)
		{
			if (!SendAsync(inSock, inpSendPacket, inPointer))
			{
				return  NetBase::PacketBase::EPacketState::Error;
			}
			return  NetBase::PacketBase::EPacketState::InComplete;
		}
		return  NetBase::PacketBase::EPacketState::Completed;
	}

	DWORD __stdcall IOCPNetworkManager::WorkerThread(LPVOID arg)
	{
		int retval;
		HANDLE hcp = (HANDLE)arg;

		while (true)
		{
			DWORD cbTransferred;
			SOCKET sock;
			NetBase::OverlappedEx* overlapped;

			retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
				(PULONG_PTR)&sock, (LPOVERLAPPED*)&overlapped, INFINITE);

			VoidPtr pointer = overlapped->pointer;
			NetBase::PacketBaseWeakPtr pPacket = overlapped->pPacket;

			try
			{
				// 비동기 입출력 결과 확인
				if (retval == 0 || cbTransferred == 0)
				{
					if (retval == 0)
					{	// error
						DWORD temp1, temp2;
						WSAGetOverlappedResult(sock,
							&overlapped->overlapped,
							&temp1, FALSE, &temp2);
						NetBase::SocketUtil::ReportError("WSAGetOverlappedResult()");
					}

					throw std::exception();
				}

				switch (overlapped->type)
				{
				case  NetBase::OverlappedEx::EOverlappedType::Accept:
				{
					NetBase::AcceptPacketPtr pAcceptPacket = std::static_pointer_cast<NetBase::AcceptPacket>(pPacket.lock());
					
					// OnAccept
					IOCPNetworkManager::sInstance->OnAccepted(pAcceptPacket);
				}
				break;

				case   NetBase::OverlappedEx::EOverlappedType::Recv:
				{
					NetBase::RecvPacketPtr pRecvPacket = std::static_pointer_cast<NetBase::RecvPacket>(pPacket.lock());
					if (false == IOCPNetworkManager::sInstance->OnRecved(sock, pRecvPacket, pointer, cbTransferred))
						continue;
					// Recv Completed
					if (false == IOCPNetworkManager::sInstance->OnCompleteRecv(pointer))
						throw std::exception();
				}
				break;

				case  NetBase::OverlappedEx::EOverlappedType::Send:
				{
					NetBase::SendPacketPtr pSendPacket = std::static_pointer_cast<NetBase::SendPacket>(pPacket.lock());
					if (false == IOCPNetworkManager::sInstance->OnSended(sock, pSendPacket, pointer, cbTransferred))
						continue;
					// Send Completed
					if (false == IOCPNetworkManager::sInstance->OnCompleteSend(pointer))
						throw std::exception();
				}
				break;
				}
			}
			catch (const std::exception&)
			{
				// OnDiscnnected
				IOCPNetworkManager::sInstance->OnDisconnected(pointer);
			}
		}

		return 0;
	}



	bool IOCPNetworkManager::DoFrame()
	{
		NetBase::SocketAddress client_addr;
		NetBase::TCPSocketPtr client_sock = m_pListenSock->Accept(client_addr);

		if (nullptr == client_sock)
			return false;

		NetBase::AcceptPacketPtr pAcceptPacket =
			NetBase::PacketManager::sInstance->GetAcceptPacketFromPool();
		pAcceptPacket->Set(client_sock, client_addr);

		PostQueuedCompletionStatus(
			*IOCPNetworkManager::sInstance->m_pHcp,
			1,
			NULL,
			&pAcceptPacket->GetOverlappedPtr()->overlapped);

		return true;
	}

	bool IOCPNetworkManager::OnRecved(SOCKET inSock, NetBase::RecvPacketPtr inpRecvPacket, VoidPtr inPointer, DWORD inCbTransferred)
	{
		NetBase::PacketBase::EPacketState result = CompleteRecv(inSock, inpRecvPacket, inPointer, inCbTransferred);

		switch (result)
		{
		case  NetBase::PacketBase::EPacketState::Error:
			//return false;
			throw std::exception();
		case  NetBase::PacketBase::EPacketState::End:
			//return false;
			throw std::exception();
		case  NetBase::PacketBase::EPacketState::InComplete:
			return false;
		case  NetBase::PacketBase::EPacketState::Completed:
			// 완료된 경우 시간을 기록
			inpRecvPacket->RecordRecvTime();
			break;
		}

		return true;
	}

	bool IOCPNetworkManager::OnSended(SOCKET inSock, NetBase::SendPacketPtr inpSendPacket, VoidPtr inPointer, DWORD inCbTransferred)
	{
		// send 완료 확인
		NetBase::PacketBase::EPacketState result = CompleteSend(inSock, inpSendPacket, inPointer, inCbTransferred);

		switch (result)
		{
		case  NetBase::PacketBase::EPacketState::Error:
			//return false;
			throw std::exception();
		case  NetBase::PacketBase::EPacketState::End:
			//return false;
			throw std::exception();
		case  NetBase::PacketBase::EPacketState::InComplete:
			return false;
		case  NetBase::PacketBase::EPacketState::Completed:
			break;
		}

		return true;
	}

	bool IOCPNetworkManager::OnCompleteRecv(VoidPtr inPointer)
	{
		IOCPSessionBasePtr pSession = std::static_pointer_cast<IOCPSessionBase>(inPointer);

		if (false == pSession->OnCompleteRecv())
			return false;
		return true;
	}

	bool IOCPNetworkManager::OnCompleteSend(VoidPtr inPointer)
	{
		IOCPSessionBasePtr pSession = std::static_pointer_cast<IOCPSessionBase>(inPointer);

		if (false == pSession->OnCompleteSend())
			return false;
		return true;
	}

	void IOCPNetworkManager::OnAccepted(NetBase::AcceptPacketPtr inpPacket)
	{
		// 새로운 클라이언트 세션 생성
		IOCPSessionBasePtr pSession = IOCPSessionManager::sInstance->CreateSession();

		pSession->SetEndPoint(inpPacket->GetPSock(), inpPacket->GetAddr());

		// thread 연결
		if (nullptr == NetBase::SocketUtil::LinkIOCPThread(inpPacket->GetPSock()->GetSock(), *m_pHcp))
			printf("thread 연결 오류 LinkIOCPThread\n");

		// accept 패킷 회수
		NetBase::PacketManager::sInstance->RetrieveAcceptPacket(inpPacket);

#ifdef __DEBUG
		::size_t ptr = (::size_t)pSession.get();
		printf("(%llu) : Session 생성\n", ptr);
#endif

		// create function 호출 시 안에서 initialize
		//pSession->Initialze();
		// 최초 recv
		if (false == pSession->Recv())
		{
			// TODO : disconnected or return false
		}
	}

	void IOCPNetworkManager::OnDisconnected(VoidPtr inPointer)
	{
		IOCPSessionBasePtr pSession = std::static_pointer_cast<IOCPSessionBase>(inPointer);
		// 세션 내부 자원 정리
		pSession->OnBeforeDisconnected();

		// session 삭제
		IOCPSessionManager::sInstance->DestroySession(pSession);

#ifdef __DEBUG
		::size_t ptr = (::size_t)pSession.get();
		printf("(%llu) : Session 삭제 완료\n", ptr);
#endif
	}
}
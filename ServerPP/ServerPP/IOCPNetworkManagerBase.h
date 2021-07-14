#pragma once

template <typename T>
class IOCPNetworkManagerBase : public NetworkManagerServer<T>
{
public:
	using psize_t = PacketManager::psize_t;
protected:
	
	HandlePtr	 m_pHcp;
	std::vector<HandlePtr> m_hAcceptThreads;
	std::vector<HandlePtr> m_hWorkerThreads;
protected:
	IOCPNetworkManagerBase() {}	
public:
	~IOCPNetworkManagerBase() {}	
	bool Initialize() override;
	void Finalize() override;
	
	HandlePtr GetHCPPtr() const;
public:
	bool RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket);	// 비동기 recv
	bool SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);	// 비동기 send	
	E_PacketState CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr& inoutpPacket, const psize_t inCompletebyte);
	E_PacketState CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const psize_t inCompletebyte);
	static DWORD WINAPI WorkerThread(LPVOID arg);
public:
	virtual bool OnAccepted(TCPSocketPtr, SocketAddress) = 0;
	virtual bool OnDisconnected() = 0;
	virtual bool OnCompleteRecv(RecvPacketPtr) = 0;
	virtual bool OnCompleteSend(SendPacketPtr) = 0;
};


template <typename T>
bool IOCPNetworkManagerBase<T>::Initialize()
{
	// iocp 입출력 포트 생성
	m_pHcp = SocketUtil::CreateIOCP(IOCPNetworkManagerBase::WorkerThread, m_hWorkerThreads);
	if (m_pHcp.get() == nullptr)
		return false;

	// parent init
	if (false == NetworkManagerServer<T>::Initialize())
		return false;

	// create accept thread
	TCPSocket* pListenSock = IOCPNetworkManagerBase<T>::sInstance->m_pListenSock.get();

	return true;
}

template <typename T>
void IOCPNetworkManagerBase<T>::Finalize()
{
}

template <typename T>
HandlePtr IOCPNetworkManagerBase<T>::GetHCPPtr() const
{
	return m_pHcp;
}

template<typename T>
bool IOCPNetworkManagerBase<T>::RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;

	// 풀링된 패킷을 꺼내오기
	RecvPacketPtr pPacket = PacketManager::sInstance->GetRecvPacketFromPool();

	// overlap wsa 준비셋팅
	pPacket->GetReady();

	// 비동기 async 수행
	retval = WSARecv(inpSock->GetSock(),
		&pPacket->m_wsabuf, 1, &recvbytes, &flags,
		&pPacket->m_overlappedEx.overlapped, nullptr);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SocketUtil::ReportError("PacketManager::RecvAsync()");
			return false;
		}
	}

	outRecvPacket = pPacket;

	return true;
}

template<typename T>
bool IOCPNetworkManagerBase<T>::SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;

	inSendPacket->GetReady(PacketManager::sInstance->m_newSendPacketID++);

	retval = WSASend(inpSock->GetSock(),
		&inSendPacket->m_wsabuf, 1, &sendbytes,
		0, &inSendPacket->m_overlappedEx.overlapped, nullptr);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SocketUtil::ReportError("PacketManager::SendAsync()");
			return false;
		}
	}

	return true;
}

template<typename T>
E_PacketState IOCPNetworkManagerBase<T>::CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr& inoutpPacket, const psize_t inCompletebyte)
{
	if (inoutpPacket->m_sizeflag)
	{
		inoutpPacket->m_recvbytes += inCompletebyte;

		if (inoutpPacket->m_recvbytes == sizeof(PacketBase::psize_t))
		{
			memcpy(&inoutpPacket->m_target_recvbytes, inoutpPacket->m_buf, sizeof(PacketBase::psize_t));
			inoutpPacket->m_recvbytes = 0;
			inoutpPacket->m_sizeflag = false;
		}

		if (!RecvAsync(inpSock, inoutpPacket))
		{
			return E_PacketState::Error;
		}

		return E_PacketState::InComplete;
	}

	inoutpPacket->m_recvbytes += inCompletebyte;

	if (inoutpPacket->m_recvbytes != inoutpPacket->m_target_recvbytes)
	{
		if (!RecvAsync(inpSock, inoutpPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}

	return E_PacketState::Completed;
}

template<typename T>
E_PacketState IOCPNetworkManagerBase<T>::CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const psize_t inCompletebyte)
{
	inpPacket->m_sendbytes += inCompletebyte;
	if (inpPacket->m_sendbytes != inpPacket->m_target_sendbytes)
	{
		if (!SendAsync(inpSock, inpPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}
	return E_PacketState::Completed;
}

template<typename T>
DWORD __stdcall IOCPNetworkManagerBase<T>::WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (true)
	{
		DWORD cbTransferred;
		typename IOCPSessionManager::clientid_t client_id = 0;
		OverlappedEx* overlapped;

		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(LPDWORD)&client_id, (LPOVERLAPPED*)&overlapped, INFINITE);

		IOCPSessionPtr pSession;
		PacketBaseWeakPtr pPacket = overlapped->pPacket;

		if (overlapped->type != E_OverlappedType::Accept)
		{
			pSession = IOCPSessionManager::sInstance->GetSession(client_id);

			// 비동기 입출력 결과 확인
			if (retval == 0 || cbTransferred == 0)
			{
				if (retval == 0)
				{
					DWORD temp1, temp2;
					WSAGetOverlappedResult(pSession->GetSockPtr()->GetSock(),
						&overlapped->overlapped,
						&temp1, FALSE, &temp2);
					SocketUtil::ReportError("WSAGetOverlappedResult()");
				}

				pSession->SetState(ESessionState::Disconnected);
			}

			if (pSession->GetState() == ESessionState::Disconnected)
			{
				pSession->OnDisconnected();
				continue;
			}
		}

		E_PacketState result;

		switch (overlapped->type)
		{
		case E_OverlappedType::Accept:
		{
			// down casting
			AcceptPacketPtr pAcceptPacket = std::static_pointer_cast<AcceptPacket>(pPacket.lock());
			pSession = IOCPSessionManager::sInstance->CreateSession();
			pSession->OnAccepted(pAcceptPacket->GetPSock(), pAcceptPacket->GetAddr());

			PacketManager::sInstance->RetrieveAcceptPacket(pAcceptPacket);
		}
		break;

		case  E_OverlappedType::Recv:
		{
			// down casting
			RecvPacketPtr pRecvPacket = std::static_pointer_cast<RecvPacket>(pPacket.lock());

			// recv 완료 확인
			result = NetworkManagerServer<T>::sInstance->CompleteRecv(
				pSession->GetSockPtr(),
				pRecvPacket,
				cbTransferred);
			switch (result)
			{
			case E_PacketState::Error:
				continue;
			case E_PacketState::End:
				continue;
			case E_PacketState::InComplete:
				continue;
			case E_PacketState::Completed:
				// 완료된 경우 시간을 기록
				pRecvPacket->RecordRecvTime();
				break;
			}

			// complete recv process
			pSession->OnCompleteRecv(pRecvPacket);

			PacketManager::sInstance->RetrieveRecvPacket(pRecvPacket);

			// recv 날려놓기
			RecvPacketPtr pOutRecvPacket;
			if (!NetworkManagerServer<T>::sInstance-> RecvAsync(pSession->GetSockPtr(), pOutRecvPacket))
			{
				continue;
			}
		}
		break;

		case E_OverlappedType::Send:
		{
			// down casting
			SendPacketPtr pSendPacket = std::static_pointer_cast<SendPacket>(pPacket.lock());

			result = NetworkManagerServer<T>::sInstance->CompleteSend(
				pSession->GetSockPtr(),
				std::static_pointer_cast<SendPacket>(pPacket.lock()),
				cbTransferred);

			switch (result)
			{
			case E_PacketState::Error:
				continue;
			case E_PacketState::End:
				continue;
			case E_PacketState::InComplete:
				continue;
			case E_PacketState::Completed:
				break;
			}

			// complete send process;
			pSession->OnCompleteSend(pSendPacket);

			// TODO : Retrieve packet
			PacketManager::sInstance->RetrieveSendPacket(pSendPacket);
		}
		break;
		}
	}

	return 0;
}

//
//
//DWORD __stdcall IOCPNetworkManager::AcceptThread(LPVOID arg)
//{
//	SocketAddress addr;
//	TCPSocket* listen_sock = (TCPSocket*)arg;
//
//	while (true)
//	{
//		// accept
//		TCPSocketPtr pClientSock = listen_sock->Accept(addr);
//
//		if (pClientSock != nullptr)
//		{// lock
//
//			// post to iocp queue
//			AcceptPacketPtr pAcceptPacket = PacketManager::sInstance->GetAcceptPacketFromPool();
//			pAcceptPacket->m_pClientSock = pClientSock;
//			PostQueuedCompletionStatus(
//				*IOCPNetworkManager::sInstance->m_pHcp,
//				1,
//				(ULONG_PTR)&pAcceptPacket,
//				&pAcceptPacket->m_overlappedEx.overlapped);
//		}// lock end
//	}
//
//	return 0;
//}
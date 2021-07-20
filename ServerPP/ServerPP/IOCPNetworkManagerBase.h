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
	bool Initialize(LPVOID) override;
	void Finalize() override;

	HandlePtr GetHCPPtr() const;
public:
	bool RecvAsync(IOCPSessionBasePtr inpSession, RecvPacketPtr inpRecvPacket);	// 비동기 recv
	bool SendAsync(IOCPSessionBasePtr inpSession, SendPacketPtr inpSendPacket);	// 비동기 send	
	E_PacketState CompleteRecv(IOCPSessionBasePtr inpSession, RecvPacketPtr inpRecvPacket, const psize_t inCompletebyte);
	E_PacketState CompleteSend(IOCPSessionBasePtr inpSession, SendPacketPtr inpSendPacket, const psize_t inCompletebyte);
	static DWORD WINAPI WorkerThread(LPVOID arg);
public:
	virtual bool DoFrame() = 0;
	virtual void OnAccepted(TCPSocketPtr inpSock, SocketAddress inSock) = 0;
	virtual void OnDisconnected(IOCPSessionBasePtr inpSession) = 0;
	virtual void OnCompleteRecv(IOCPSessionBasePtr inpSession, RecvPacketPtr inpPacket) = 0;
	virtual void OnCompleteSend(IOCPSessionBasePtr inpSession, SendPacketPtr inpPacket) = 0;
};


template <typename T>
bool IOCPNetworkManagerBase<T>::Initialize(LPVOID)
{
	// iocp 입출력 포트 생성
	m_pHcp = SocketUtil::CreateIOCP(IOCPNetworkManagerBase::WorkerThread, m_hWorkerThreads);
	if (m_pHcp.get() == nullptr)
		return false;

	// parent init
	if (false == NetworkManagerServer<T>::Initialize(nullptr))
		return false;

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
bool IOCPNetworkManagerBase<T>::RecvAsync(IOCPSessionBasePtr inpSession, RecvPacketPtr inpRecvPacket)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;
	TCPSocketPtr pSock = inpSession->GetSockPtr();
	
	// overlap wsa 준비셋팅
	inpRecvPacket->GetReady();
	inpRecvPacket->m_overlappedEx.pointer = inpSession;

	// 비동기 async 수행
	retval = WSARecv(pSock->GetSock(),
		&inpRecvPacket->m_wsabuf, 1, &recvbytes, &flags,
		&inpRecvPacket->m_overlappedEx.overlapped, nullptr);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SocketUtil::ReportError("PacketManager::RecvAsync()");
			return false;
		}
	}	

	return true;
}

template<typename T>
bool IOCPNetworkManagerBase<T>::SendAsync(IOCPSessionBasePtr inpSession, SendPacketPtr inpSendPacket)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;
	TCPSocketPtr pSock = inpSession->GetSockPtr();

	inpSendPacket->GetReady(inpSession->m_newSendID++);
	inpSendPacket->m_overlappedEx.pointer = inpSession;

	retval = WSASend(pSock->GetSock(),
		&inpSendPacket->m_wsabuf, 1, &sendbytes,
		0, &inpSendPacket->m_overlappedEx.overlapped, nullptr);

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
E_PacketState IOCPNetworkManagerBase<T>::CompleteRecv(IOCPSessionBasePtr inpSession, RecvPacketPtr inpRecvPacket, const psize_t inCompletebyte)
{
	if (inpRecvPacket->m_sizeflag)
	{
		inpRecvPacket->m_recvbytes += inCompletebyte;

		if (inpRecvPacket->m_recvbytes == sizeof(PacketBase::psize_t))
		{
			memcpy(&inpRecvPacket->m_target_recvbytes, inpRecvPacket->m_buf, sizeof(PacketBase::psize_t));
			inpRecvPacket->m_recvbytes = 0;
			inpRecvPacket->m_sizeflag = false;
		}

		if (!RecvAsync(inpSession, inpRecvPacket))
		{
			return E_PacketState::Error;
		}

		return E_PacketState::InComplete;
	}

	inpRecvPacket->m_recvbytes += inCompletebyte;

	if (inpRecvPacket->m_recvbytes != inpRecvPacket->m_target_recvbytes)
	{
		if (!RecvAsync(inpSession, inpRecvPacket))
		{
			return E_PacketState::Error;
		}
		return E_PacketState::InComplete;
	}

	inpRecvPacket->UnPacking();

	// duplicate check
	if (inpRecvPacket->GetId() < inpSession->m_newRecvID)
		return E_PacketState::Duplicated;

	++inpSession->m_newRecvID;
	return E_PacketState::Completed;
}

template<typename T>
E_PacketState IOCPNetworkManagerBase<T>::CompleteSend(IOCPSessionBasePtr inpSession, SendPacketPtr inpPacket, const psize_t inCompletebyte)
{
	inpPacket->m_sendbytes += inCompletebyte;
	if (inpPacket->m_sendbytes != inpPacket->m_target_sendbytes)
	{
		if (!SendAsync(inpSession, inpPacket))
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
		TCPSocketPtr pSock;
		OverlappedEx* overlapped;

		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&pSock, (LPOVERLAPPED*)&overlapped, INFINITE);


		IOCPSessionBasePtr pIOCPSession;
		if (nullptr != overlapped->pointer)
			pIOCPSession = std::static_pointer_cast<IOCPSessionBase>(overlapped->pointer);
		PacketBaseWeakPtr pPacket = overlapped->pPacket;

		if (overlapped->type != E_OverlappedType::Accept)
		{
			// 비동기 입출력 결과 확인
			if (retval == 0 || cbTransferred == 0)
			{
				if (retval == 0)
				{
					DWORD temp1, temp2;
					WSAGetOverlappedResult(pSock->GetSock(),
						&overlapped->overlapped,
						&temp1, FALSE, &temp2);
					SocketUtil::ReportError("WSAGetOverlappedResult()");
				}

				//pSession->SetState(ESessionState::Disconnected);
				IOCPNetworkManagerBase<T>::sInstance->OnDisconnected(pIOCPSession);
				continue;
			}

			/*if (pSession->GetState() == ESessionState::Disconnected)
			{
				pSession->OnDisconnected();
				continue;
			}*/
		}

		E_PacketState result;

		switch (overlapped->type)
		{
		case E_OverlappedType::Accept:
		{
			// down casting
			AcceptPacketPtr pAcceptPacket = std::static_pointer_cast<AcceptPacket>(pPacket.lock());
			IOCPNetworkManagerBase<T>::sInstance->OnAccepted(pAcceptPacket->GetPSock(), pAcceptPacket->GetAddr());

			PacketManager::sInstance->RetrieveAcceptPacket(pAcceptPacket);
		}
		break;

		case  E_OverlappedType::Recv:
		{
			// down casting
			RecvPacketPtr pRecvPacket = std::static_pointer_cast<RecvPacket>(pPacket.lock());

			// recv 완료 확인
			result = IOCPNetworkManagerBase<T>::sInstance->CompleteRecv(
				pIOCPSession,
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
			IOCPNetworkManagerBase<T>::sInstance->OnCompleteRecv(pIOCPSession, pRecvPacket);

			PacketManager::sInstance->RetrieveRecvPacket(pRecvPacket);

			// recv 날려놓기
			RecvPacketPtr pNewRecvPacket = PacketManager::sInstance->GetRecvPacketFromPool();
			if (!IOCPNetworkManagerBase<T>::sInstance->RecvAsync(pIOCPSession, pNewRecvPacket))
			{
				continue;
			}
		}
		break;

		case E_OverlappedType::Send:
		{
			// down casting
			SendPacketPtr pSendPacket = std::static_pointer_cast<SendPacket>(pPacket.lock());

			result = IOCPNetworkManagerBase<T>::sInstance->CompleteSend(
				pIOCPSession,
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
			IOCPNetworkManagerBase<T>::sInstance->OnCompleteSend(pIOCPSession, pSendPacket);

			// TODO : Retrieve packet
			PacketManager::sInstance->RetrieveSendPacket(pSendPacket);
		}
		break;
		}
	}

	return 0;
}
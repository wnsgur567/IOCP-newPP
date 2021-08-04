#pragma once

template <typename T>
class IOCPNetworkManagerBase : public NetworkManagerServer<T>
{
public:
	using packetSize_t = PacketManager::packetSize_t;
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
	bool RecvAsync(SOCKET, RecvPacketPtr, VoidPtr);	// 비동기 recv
	bool SendAsync(SOCKET, SendPacketPtr, VoidPtr);	// 비동기 send	
	static DWORD WINAPI WorkerThread(LPVOID arg);
protected:
	PacketBase::EPacketState CompleteRecv(SOCKET, RecvPacketPtr, VoidPtr, const packetSize_t);
	PacketBase::EPacketState CompleteSend(SOCKET, SendPacketPtr, VoidPtr, const packetSize_t);
protected:
	virtual bool DoFrame() = 0;

	virtual bool OnRecved(SOCKET, RecvPacketPtr, VoidPtr, DWORD) = 0;
	virtual bool OnSended(SOCKET, SendPacketPtr, VoidPtr, DWORD) = 0;
	virtual bool OnCompleteRecv(VoidPtr) = 0;
	virtual bool OnCompleteSend(VoidPtr) = 0;

	virtual void OnAccepted(AcceptPacketPtr) = 0;
	virtual void OnDisconnected(VoidPtr) = 0;
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
inline bool IOCPNetworkManagerBase<T>::RecvAsync(SOCKET inSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer)
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
			SocketUtil::ReportError("PacketManager::RecvAsync()");
			return false;
		}
	}

	return true;
}

template<typename T>
inline bool IOCPNetworkManagerBase<T>::SendAsync(SOCKET inSock, SendPacketPtr inpSendPacket, VoidPtr inPointer)
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
			SocketUtil::ReportError("PacketManager::SendAsync()");
			return false;
		}
	}

	return true;
}

template<typename T>
inline PacketBase::EPacketState IOCPNetworkManagerBase<T>::CompleteRecv(SOCKET inSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer, const packetSize_t inCompletebyte)
{
	BYTE* ptr = inpRecvPacket->m_pStream->m_buffer;

	if (inpRecvPacket->m_sizeflag)
	{
		inpRecvPacket->m_recvbytes += inCompletebyte;

		if (inpRecvPacket->m_recvbytes == sizeof(PacketBase::packetSize_t))
		{
			memcpy(&inpRecvPacket->m_target_recvbytes, ptr, sizeof(PacketBase::packetSize_t));
			inpRecvPacket->m_recvbytes = 0;
			inpRecvPacket->m_sizeflag = false;
		}

		if (false == RecvAsync(inSock, inpRecvPacket, inPointer))
		{
			return PacketBase::EPacketState::Error;
		}

		return PacketBase::EPacketState::InComplete;
	}

	inpRecvPacket->m_recvbytes += inCompletebyte;

	if (inpRecvPacket->m_recvbytes != inpRecvPacket->m_target_recvbytes)
	{
		if (false == RecvAsync(inSock, inpRecvPacket, inPointer))
		{
			return PacketBase::EPacketState::Error;
		}
		return PacketBase::EPacketState::InComplete;
	}

	return PacketBase::EPacketState::Completed;
}

template<typename T>
inline PacketBase::EPacketState IOCPNetworkManagerBase<T>::CompleteSend(SOCKET inSock, SendPacketPtr inpSendPacket, VoidPtr inPointer, const packetSize_t inCompletebyte)
{
	inpSendPacket->m_sendbytes += inCompletebyte;
	if (inpSendPacket->m_sendbytes != inpSendPacket->m_target_sendbytes)
	{
		if (!SendAsync(inSock, inpSendPacket, inPointer))
		{
			return PacketBase::EPacketState::Error;
		}
		return PacketBase::EPacketState::InComplete;
	}
	return PacketBase::EPacketState::Completed;
}

template<typename T>
DWORD __stdcall IOCPNetworkManagerBase<T>::WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (true)
	{
		DWORD cbTransferred;
		SOCKET sock;
		OverlappedEx* overlapped;

		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&sock, (LPOVERLAPPED*)&overlapped, INFINITE);

		VoidPtr pointer = overlapped->pointer;
		PacketBaseWeakPtr pPacket = overlapped->pPacket;

		// 비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{	// error
				DWORD temp1, temp2;
				WSAGetOverlappedResult(sock,
					&overlapped->overlapped,
					&temp1, FALSE, &temp2);
				SocketUtil::ReportError("WSAGetOverlappedResult()");
			}

			// OnDiscnnected
			IOCPNetworkManagerBase<T>::sInstance->OnDisconnected(pointer);
			continue;
		}


		switch (overlapped->type)
		{
		case OverlappedEx::EOverlappedType::Accept:
		{
			AcceptPacketPtr pAcceptPacket = std::static_pointer_cast<AcceptPacket>(pPacket.lock());
			// OnAccept
			IOCPNetworkManagerBase<T>::sInstance->OnAccepted(pAcceptPacket);
		}
		break;

		case  OverlappedEx::EOverlappedType::Recv:
		{
			RecvPacketPtr pRecvPacket = std::static_pointer_cast<RecvPacket>(pPacket.lock());
			if (false == IOCPNetworkManagerBase<T>::sInstance->OnRecved(sock, pRecvPacket, pointer, cbTransferred))
				continue;
			// Recv Completed
			IOCPNetworkManagerBase<T>::sInstance->OnCompleteRecv(pointer);

		}
		break;

		case OverlappedEx::EOverlappedType::Send:
		{
			SendPacketPtr pSendPacket = std::static_pointer_cast<SendPacket>(pPacket.lock());
			if (false == IOCPNetworkManagerBase<T>::sInstance->OnSended(sock, pSendPacket, pointer, cbTransferred))
				continue;
			// Send Completed
			IOCPNetworkManagerBase<T>::sInstance->OnCompleteSend(pointer);
		}
		break;
		}
	}

	return 0;
}
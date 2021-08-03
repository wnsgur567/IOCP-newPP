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
	bool RecvAsync(TCPSocketPtr inpSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer);	// �񵿱� recv
	bool SendAsync(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket, VoidPtr inPointer);	// �񵿱� send	
	static DWORD WINAPI WorkerThread(LPVOID arg);
protected:
	PacketBase::EPacketState CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer, const packetSize_t inCompletebyte);
	PacketBase::EPacketState CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket, VoidPtr inPointer, const packetSize_t inCompletebyte);
protected:
	virtual bool DoFrame() = 0;

	virtual bool OnRecved(TCPSocketPtr, RecvPacketPtr, VoidPtr, DWORD) = 0;
	virtual bool OnSended(TCPSocketPtr, SendPacketPtr, VoidPtr, DWORD) = 0;
	virtual bool OnCompleteRecv(VoidPtr) = 0;
	virtual bool OnCompleteSend(VoidPtr) = 0;

	virtual void OnAccepted(AcceptPacketPtr) = 0;
	virtual void OnDisconnected(VoidPtr) = 0;
};


template <typename T>
bool IOCPNetworkManagerBase<T>::Initialize(LPVOID)
{
	// iocp ����� ��Ʈ ����
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
inline bool IOCPNetworkManagerBase<T>::RecvAsync(TCPSocketPtr inpSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer)
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;

	// overlap wsa �غ����
	inpRecvPacket->GetReady();
	inpRecvPacket->m_overlappedEx.pointer = inPointer;

	// �񵿱� async ����
	retval = WSARecv(inpSock->GetSock(),
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
inline bool IOCPNetworkManagerBase<T>::SendAsync(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket, VoidPtr inPointer)
{
	int retval;
	DWORD sendbytes;
	DWORD flags = 0;

	inpSendPacket->GetReady();
	inpSendPacket->m_overlappedEx.pointer = inPointer;

	retval = WSASend(inpSock->GetSock(),
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
inline PacketBase::EPacketState IOCPNetworkManagerBase<T>::CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr inpRecvPacket, VoidPtr inPointer, const packetSize_t inCompletebyte)
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

		if (false == RecvAsync(inpSock, inpRecvPacket, inPointer))
		{
			return PacketBase::EPacketState::Error;
		}

		return PacketBase::EPacketState::InComplete;
	}

	inpRecvPacket->m_recvbytes += inCompletebyte;

	if (inpRecvPacket->m_recvbytes != inpRecvPacket->m_target_recvbytes)
	{
		if (false == RecvAsync(inpSock, inpRecvPacket, inPointer))
		{
			return PacketBase::EPacketState::Error;
		}
		return PacketBase::EPacketState::InComplete;
	}

	return PacketBase::EPacketState::Completed;
}

template<typename T>
inline PacketBase::EPacketState IOCPNetworkManagerBase<T>::CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpSendPacket, VoidPtr inPointer, const packetSize_t inCompletebyte)
{
	inpSendPacket->m_sendbytes += inCompletebyte;
	if (inpSendPacket->m_sendbytes != inpSendPacket->m_target_sendbytes)
	{
		if (!SendAsync(inpSock, inpSendPacket, inPointer))
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
		TCPSocketPtr pSock;
		OverlappedEx* overlapped;

		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(PULONG_PTR)&pSock, (LPOVERLAPPED*)&overlapped, INFINITE);

		VoidPtr pointer = overlapped->pointer;
		PacketBaseWeakPtr pPacket = overlapped->pPacket;

		// �񵿱� ����� ��� Ȯ��
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{	// error
				DWORD temp1, temp2;
				WSAGetOverlappedResult(pSock->GetSock(),
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
			if (false == IOCPNetworkManagerBase<T>::sInstance->OnRecved(pSock, pRecvPacket, pointer, cbTransferred))
				continue;
			// Recv Completed
			IOCPNetworkManagerBase<T>::sInstance->OnCompleteRecv(pointer);

		}
		break;

		case OverlappedEx::EOverlappedType::Send:
		{
			SendPacketPtr pSendPacket = std::static_pointer_cast<SendPacket>(pPacket.lock());
			if (false == IOCPNetworkManagerBase<T>::sInstance->OnSended(pSock, pSendPacket, pointer, cbTransferred))
				continue;
			// Send Completed
			IOCPNetworkManagerBase<T>::sInstance->OnCompleteSend(pointer);
		}
		break;
		}
	}

	return 0;
}
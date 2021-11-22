#pragma once

namespace IOCP_Base
{
	// VoidPtr will be casting to 'IOCPSessionBasePtr'

	class IOCPNetworkManager : public NetworkManagerServer<IOCPNetworkManager>
	{
		friend class Singleton;
	public:
		using packetSize_t = NetBase::PacketManager::packetSize_t;
		using InitArgs = NetworkManagerServer::InitArgs;
	protected:
		// IOCP 객체
		HandlePtr	 m_pHcp;
		// Accept threads
		std::vector<HandlePtr> m_hAcceptThreads;
		// Worker threads
		std::vector<HandlePtr> m_hWorkerThreads;
		// DB threads
		std::vector<HandlePtr> m_hDBThreads;
	protected:
		IOCPNetworkManager() {}
	public:
		~IOCPNetworkManager() {}
		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;

		HandlePtr GetHCPPtr() const;
	public:
		bool DoFrame();

		bool RecvAsync(SOCKET, NetBase::RecvPacketPtr, VoidPtr);	// 비동기 recv
		bool SendAsync(SOCKET, NetBase::SendPacketPtr, VoidPtr);	// 비동기 send	
		static DWORD WINAPI WorkerThread(LPVOID arg);	// worker thread
	protected:
		NetBase::PacketBase::EPacketState CompleteRecv(SOCKET, NetBase::RecvPacketPtr, VoidPtr, const packetSize_t);
		NetBase::PacketBase::EPacketState CompleteSend(SOCKET, NetBase::SendPacketPtr, VoidPtr, const packetSize_t);
	protected:		

		bool OnRecved(SOCKET, NetBase::RecvPacketPtr, VoidPtr, DWORD);
		bool OnSended(SOCKET, NetBase::SendPacketPtr, VoidPtr, DWORD);
		bool OnCompleteRecv(VoidPtr);
		bool OnCompleteSend(VoidPtr);
				
		void OnAccepted(NetBase::AcceptPacketPtr);
		void OnDisconnected(VoidPtr);
	};	
	
}
#pragma once

namespace IOCP_Base
{
	class IOCPSessionBase;
	using IOCPSessionBasePtr = std::shared_ptr<IOCPSessionBase>;

	class IOCPSessionBase
	{
	private:
		friend class IOCPNetworkManager;
	public:
		using packetSize_t = NetBase::PacketBase::packetSize_t;
		using pid_t = NetBase::PacketBase::packetId_t;
	protected:
		NetBase::SocketAddress m_addr;
		NetBase::TCPSocketPtr m_pSock;

		pid_t m_newSendID;
		pid_t m_newRecvID;
		NetBase::RecvPacketPtr m_pRecvPacket;
		std::queue<NetBase::SendPacketPtr> m_sendPacketQueue;
		MyBase::CriticalSection m_cs;	// critical_section for sendqueue

		IOCPSessionBase();
		NetBase::TCPSocketPtr GetSockPtr();
	public:
		virtual ~IOCPSessionBase() {}
		IOCPSessionBase(const IOCPSessionBase&) = delete;
		IOCPSessionBase& operator=(const IOCPSessionBase&) = delete;
		void SetEndPoint(NetBase::TCPSocketPtr inpSock, NetBase::SocketAddress inAddress);
		virtual void Initialze() = 0;
	public:
		virtual bool Recv() = 0;
		virtual bool Send(NetBase::OutputMemoryStreamPtr pStream) = 0;
		virtual bool OnCompleteRecv() = 0;
		virtual bool OnCompleteSend() = 0;
		virtual void OnBeforeDisconnected() = 0;
	};
}
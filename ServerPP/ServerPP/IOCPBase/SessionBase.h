#pragma once

namespace IOCP_Base
{

	class SessionBase;
	using SessionBasePtr = std::shared_ptr<SessionBase>;

	class SessionBase
	{
		friend class IOCPNetworkManager;
		
	public:
		using packetSize_t = NetBase::PacketBase::packetSize_t;
		using pid_t = NetBase::PacketBase::packetId_t;
	protected:
		NetBase::TCPSocketPtr m_pSock;
		NetBase::SocketAddress m_addr;

		pid_t m_newSendID;
		pid_t m_newRecvID;
		SessionBase();
	public:
		virtual ~SessionBase();

		SessionBase(const SessionBase&) = delete;
		SessionBase& operator=(const SessionBase&) = delete;
	protected:
		NetBase::TCPSocketPtr GetSockPtr();
	protected:
		virtual void Initialze() = 0;
	};
}

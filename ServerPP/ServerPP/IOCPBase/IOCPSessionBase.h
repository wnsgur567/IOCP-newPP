#pragma once

namespace IOCP_Base
{
	class IOCPSessionBase;
	using IOCPSessionBasePtr = std::shared_ptr<IOCPSessionBase>;

	class IOCPSessionBase : public SessionBase
	{
	private:
		template<typename T>
		friend class IOCPNetworkManagerBase;
	protected:
		NetBase::RecvPacketPtr m_pRecvPacket;
		std::queue<NetBase::SendPacketPtr> m_sendPacketQueue;
		IOCPSessionBase();
	public:
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
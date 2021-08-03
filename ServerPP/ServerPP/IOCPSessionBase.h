#pragma once

class IOCPSessionBase;
using IOCPSessionBasePtr = std::shared_ptr<IOCPSessionBase>;

class IOCPSessionBase : public SessionBase
{
private:	
	template<typename T>
	friend class IOCPNetworkManagerBase;
protected:
	virtual void Initialze() = 0;
	RecvPacketPtr m_pRecvPacket;
	std::queue<SendPacketPtr> m_sendPacketQueue;
public:
	IOCPSessionBase();
	virtual bool Recv() = 0;
	virtual bool Send(SendPacketPtr pSendPacket) = 0;
	virtual bool OnCompleteRecv() = 0;
	virtual bool OnCompleteSend() = 0;
};
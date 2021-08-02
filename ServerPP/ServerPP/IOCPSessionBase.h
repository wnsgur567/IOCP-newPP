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
	RecvPacketPtr m_recvPacket;
	std::queue<SendPacketPtr> m_sendPacketQueue;
public:
	IOCPSessionBase();
	virtual bool Recv() = 0;
	virtual bool Send() = 0;
	virtual bool OnCompleteRecv() = 0;
	virtual bool OnCompleteSend() = 0;
};
#pragma once

class IOCPNetworkManager : public IOCPNetworkManagerBase<IOCPNetworkManager>
{
public:
	virtual bool OnAccepted(TCPSocketPtr, SocketAddress) override;
	virtual bool OnDisconnected() override;
	virtual bool OnCompleteRecv(RecvPacketPtr) override;
	virtual bool OnCompleteSend(SendPacketPtr) override;
};
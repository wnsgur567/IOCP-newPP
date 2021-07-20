#pragma once

class IOCPNetworkManager : public IOCPNetworkManagerBase<IOCPNetworkManager>
{
public:
	bool Initialize(LPVOID) override;
	void Finalize() override;
public:
	bool DoFrame() override;
	void OnAccepted(TCPSocketPtr, SocketAddress) override;
	void OnDisconnected(IOCPSessionBasePtr inpSession) override;
	void OnCompleteRecv(IOCPSessionBasePtr inpSession, RecvPacketPtr inpPacket) override;
	void OnCompleteSend(IOCPSessionBasePtr inpSession, SendPacketPtr inpPacket) override;
};
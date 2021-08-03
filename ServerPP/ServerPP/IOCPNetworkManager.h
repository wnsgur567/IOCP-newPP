#pragma once

class IOCPNetworkManager : public IOCPNetworkManagerBase<IOCPNetworkManager>
{
public:
	bool Initialize(LPVOID) override;
	void Finalize() override;
public:
	bool DoFrame() override;

	bool OnRecved(TCPSocketPtr, RecvPacketPtr, VoidPtr, DWORD) override;
	bool OnSended(TCPSocketPtr, SendPacketPtr, VoidPtr, DWORD) override;
	bool OnCompleteRecv(VoidPtr) override;
	bool OnCompleteSend(VoidPtr) override;

	void OnAccepted(AcceptPacketPtr) override;
	void OnDisconnected(VoidPtr) override;
};
#pragma once



class NetworkManagerClient : public NetworkManagerClientBase<NetworkManagerClient>
{
	friend class Singleton;	
protected:
	NetworkManagerClient() {}

public:

	bool DoFrame() override;
	// �ϴ� ����� ����
	bool Recv(RecvPacketPtr& outRecvPacket) override;
	bool Send(SendPacketPtr inpPacket) override;
};
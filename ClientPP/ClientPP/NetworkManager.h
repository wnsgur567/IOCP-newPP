#pragma once



class NetworkManagerClient : public NetworkManagerClientBase<NetworkManagerClient>
{
	friend class Singleton;	
protected:
	NetworkManagerClient() {}

public:

	bool DoFrame() override;
	// 일단 동기로 제작
	bool Recv(RecvPacketPtr& outRecvPacket) override;
	bool Send(SendPacketPtr inpPacket) override;
};
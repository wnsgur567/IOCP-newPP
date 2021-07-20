#pragma once

class NetworkEngine : public NetworkEngineBase<NetworkEngine, NetworkManagerClient, SessionManager, Session>
{
public:
	bool DoFrame() override;
};
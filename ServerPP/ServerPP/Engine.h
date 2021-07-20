#pragma once

class Engine : public EngineBase<Engine, IOCPNetworkManager, IOCPSessionManager, IOCPSession>
{
	friend class Singleton;
protected:
	Engine() {}
public:
	bool DoFrame() override;	
};
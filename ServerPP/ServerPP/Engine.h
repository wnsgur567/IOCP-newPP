#pragma once

class Engine : public NetIOCP::EngineBase<Engine, IOCPSession>
{
	friend class Singleton;
protected:
	Engine() {}
public:
	bool DoFrame() override;
};
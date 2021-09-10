#pragma once

class Engine : public IOCP_Base::EngineBase<Engine, IOCPSession>
{
	friend class Singleton;
protected:
	Engine() {}
public:
	bool DoFrame() override;
	bool Initialize(LPVOID args) noexcept override;
	void Finalize() noexcept override;
};
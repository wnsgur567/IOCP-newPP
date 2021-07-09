#pragma once

class Engine : public Singleton<Engine>
{
	friend class Singleton;
protected:
	Engine() {}
public:
	bool Initialize() override;
	void Finalize() override;

	bool DoFrame();
	bool DoLoop();
};
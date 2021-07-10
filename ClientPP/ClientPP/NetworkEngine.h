#pragma once

class NetworkEngine : public Singleton<NetworkEngine>
{
	friend class Singleton;
protected:
public:
	bool Initialize() override;
	void Finalize() override;
};
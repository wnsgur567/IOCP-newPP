#pragma once

class SQLManagerEx : public MyBase::Singleton<SQLManagerEx>
{
	bool bLoop;
	ThreadSafeQueue<NetBase::SQLPacketPtr> queue;
private:
	friend Singleton;
	SQLManagerEx() : bLoop(true) {}
public:
	virtual bool Initialize(LPVOID) noexcept override;
	virtual void Finalize() noexcept override;
public:
	void Request(NetBase::SQLPacketPtr queryPacket);
	DWORD __stdcall DBThread(LPVOID arg);
};
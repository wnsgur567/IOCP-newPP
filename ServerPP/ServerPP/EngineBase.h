#pragma once

template <typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
class EngineBase : public Singleton<DerivedEngine>
{
public:
	virtual bool DoFrame() = 0;
	bool DoLoop();
	bool Initialize(LPVOID) override;
	void Finalize() override;
};


template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
inline bool EngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::DoLoop()
{
	while (true)
	{
		if (false == DoFrame())
			break;
	}

	return true;
}

template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
inline bool EngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::Initialize(LPVOID)
{
	if (false == DerivedSessionManager::StaticInit())
		return false;
	DerivedSessionManager::sInstance->Initialize(nullptr);
	DerivedSessionManager::sInstance->RegistCreationFunction(SessionBase::CreateSession<DerivedSession>);

	if (false == DerivedNetworkManager::StaticInit())
		return false;
	DerivedNetworkManager::sInstance->Initialize(nullptr);

	// 암호 매니저
	if (false == CipherManager::StaticInit())
		return false;
	CipherManager::sInstance->Initialize(nullptr);

	// 패킷 매니저
	PacketManager::InitializeArgs packetArgs;
	packetArgs.capacityOfRecvBuffer = packetArgs.capacityOfSendBuffer = BUFSIZE;
	packetArgs.numberOfAcptPacket = packetArgs.numberOfRecvPacket = packetArgs.numberOfSendPacket = 1024;
	if (false == PacketManager::StaticInit())
		return false;
	PacketManager::sInstance->Initialize(&packetArgs);

	return true;
}

template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
inline void EngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::Finalize()
{
}

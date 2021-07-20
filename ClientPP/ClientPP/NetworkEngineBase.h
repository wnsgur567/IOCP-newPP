#pragma once

template <typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
class NetworkEngineBase : public Singleton<DerivedEngine>
{
public:
	virtual bool DoFrame() = 0;
	bool DoLoop();
	bool Initialize(LPVOID) override;
	void Finalize() override;
};

//template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
//inline bool NetworkEngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::DoFrame()
//{
//	if (false == DerivedNetworkManager::sInstance->DoFrame())
//		return false;
//	return true;
//}

template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
inline bool NetworkEngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::DoLoop()
{
	while (true)
	{
		if (false == DoFrame())
			break;
	}

	return true;
}

template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
inline bool NetworkEngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::Initialize(LPVOID)
{
	if (false == DerivedSessionManager::StaticInit())
		return false;
	DerivedSessionManager::sInstance->Initialize(nullptr);
	DerivedSessionManager::sInstance->RegistCreationFunction(SessionBase::CreateSession<DerivedSession>);
	std::shared_ptr<DerivedSession> pSessionBase = DerivedSessionManager::sInstance->CreateSession<DerivedSession>();

	if (false == DerivedNetworkManager::StaticInit())
		return false;
	DerivedNetworkManager::sInstance->Initialize(&pSessionBase);

	if (false == PacketManager::StaticInit())
		return false;
	PacketManager::sInstance->Initialize(nullptr);

	return true;
}

template<typename DerivedEngine, typename DerivedNetworkManager, typename DerivedSessionManager, typename DerivedSession>
inline void NetworkEngineBase<DerivedEngine, DerivedNetworkManager, DerivedSessionManager, DerivedSession>::Finalize()
{
	//...	
	PacketManager::sInstance->Finalize();
	DerivedSessionManager::sInstance->Finalize();
	DerivedSessionManager::sInstance->Finalize();
}

#pragma once

#define DERIVED_ENGINE_FRIEND_CODE template<typename DerivedEngine, typename DerivedSession> friend class NetIOCP::EngineBase

namespace IOCP_Base
{
	template <typename DerivedEngine, typename DerivedSession>
	class EngineBase : public MyBase::Singleton<DerivedEngine>
	{
	public:
		virtual bool DoFrame() = 0;
		bool DoLoop();
		bool Initialize(LPVOID) override;
		void Finalize() override;
	};


	template<typename DerivedEngine, typename DerivedSession>
	inline bool EngineBase<DerivedEngine, DerivedSession>::DoLoop()
	{
		while (true)
		{
			if (IOCPNetworkManager::sInstance->DoFrame())
				break;

			if (false == DoFrame())
				break;
		}

		return true;
	}

	template<typename DerivedEngine, typename DerivedSession>
	inline bool EngineBase<DerivedEngine, DerivedSession>::Initialize(LPVOID)
	{
		if (false == IOCPSessionManager::StaticInit())
			return false;		

		IOCPSessionManager::sInstance->Initialize(nullptr);
		IOCPSessionManager::sInstance->RegistCreationFunction(DerivedSession::CreateSession);

		if (false == IOCPNetworkManager::StaticInit())
			return false;
		IOCPNetworkManager::sInstance->Initialize(nullptr);

		// 암호 매니저
		if (false == NetCipher::CipherManager::StaticInit())
			return false;

		NetCipher::CipherManager::sInstance->Initialize(nullptr);

		// 패킷 매니저
		NetBase::PacketManager::InitializeArgs packetArgs;
		packetArgs.capacityOfRecvBuffer = packetArgs.capacityOfSendBuffer = BUFSIZE;
		packetArgs.numberOfAcptPacket = packetArgs.numberOfRecvPacket = packetArgs.numberOfSendPacket = 1024;
		if (false == NetBase::PacketManager::StaticInit())
			return false;
		NetBase::PacketManager::sInstance->Initialize(&packetArgs);

		return true;
	}

	template<typename DerivedEngine, typename DerivedSession>
	inline void EngineBase<DerivedEngine, DerivedSession>::Finalize()
	{
	}
}

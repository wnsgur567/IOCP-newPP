#pragma once

#define DERIVED_ENGINE_FRIEND_CODE template<typename DerivedEngine, typename DerivedSession> friend class IOCP_Base::EngineBase

namespace IOCP_Base
{
	template <typename DerivedEngine, typename DerivedSession>
	class EngineBase : public MyBase::Singleton<DerivedEngine>
	{
	public:
		struct InitArgs
		{
			NetBase::PacketManager::packetSize_t stream_buf_size;
			gsize64_t acptpacket_pool_size;
			gsize64_t recvpacket_pool_size;
			gsize64_t sendpacket_pool_size;
		};
	public:
		virtual bool DoFrame() = 0;
		bool DoLoop();
		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;
	};


	template<typename DerivedEngine, typename DerivedSession>
	inline bool EngineBase<DerivedEngine, DerivedSession>::DoLoop()
	{
		while (true)
		{
			if (false == IOCPNetworkManager::sInstance->DoFrame())
				break;

			if (false == DoFrame())
				break;
		}

		return true;
	}

	template<typename DerivedEngine, typename DerivedSession>
	inline bool EngineBase<DerivedEngine, DerivedSession>::Initialize(LPVOID agrs) noexcept
	{
		InitArgs* pArgs = (InitArgs*)agrs;

		if (false == IOCPSessionManager::StaticInit())
			return false;

		IOCPSessionManager::sInstance->Initialize(nullptr);
		IOCPSessionManager::sInstance->RegistCreationFunction(DerivedSession::CreateSession);

		IOCPNetworkManager::InitArgs netArgs;
		netArgs.ip = "";
		netArgs.port = 9000;
		if (false == IOCPNetworkManager::StaticInit())
			return false;
		IOCPNetworkManager::sInstance->Initialize(&netArgs);

		// 암호 매니저
		if (false == NetCipher::CipherManager::StaticInit())
			return false;
		NetCipher::CipherManager::sInstance->Initialize(nullptr);

		// 패킷 매니저
		NetBase::PacketManager::InitializeArgs packetArgs;
		packetArgs.capacityOfRecvBuffer = packetArgs.capacityOfSendBuffer = pArgs->stream_buf_size;
		packetArgs.numberOfAcptPacket = pArgs->acptpacket_pool_size;
		packetArgs.numberOfRecvPacket = pArgs->recvpacket_pool_size;
		packetArgs.numberOfSendPacket = pArgs->sendpacket_pool_size;
		if (false == NetBase::PacketManager::StaticInit())
			return false;
		NetBase::PacketManager::sInstance->Initialize(&packetArgs);

		return true;
	}

	template<typename DerivedEngine, typename DerivedSession>
	inline void EngineBase<DerivedEngine, DerivedSession>::Finalize() noexcept
	{
	}
}

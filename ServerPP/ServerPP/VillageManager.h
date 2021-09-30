#pragma once
namespace Village
{

	class VillageManager : public MyBase::Singleton<VillageManager>
	{
		friend class Singleton;
	public:
		using ESectorProtocol = SectorManager::EProtocol;
		enum class EProtocol : ProtocolSize_t
		{
			None = 0,

			//VillageInit = 1 << 0,						// village 정보 전송
			EnterVillage = 1ULL << 1,					// village 입장
			ExitVillage = 1ULL << 2,					// village 퇴장
		};
		enum class EResult : ResultSize_t
		{
			None,

			
		};	
	protected:
		VillageManager() {}
		// db 로부터 불러올 정보 container
		std::unordered_map < uint32_t, VillageInfoBasePtr> m_villageInfo_map;
	public:
		~VillageManager() {	}
		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;
		VillageInfoBasePtr GetVillageInfo(uint32_t inVillageID);
	public:
		EResult StateChangedProcess(IOCPSessionPtr inpSession);			// current village info
		EResult VillageEnterProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);		// current village info
		EResult VillageExitProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);
		EResult VillageActionAndMoveProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);
		EResult VillageActionProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);
		EResult VillageMoveProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);
		EResult GoBackToCharacterSelectProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);		//user id
	};
}


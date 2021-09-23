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
			EnterVillage = 1 << 1,						// village 입장
			ExitVillage = 1 << 2,						// village 퇴장

			// 20 영역부터 ESectorProtocol 에서 사용
			//PlayerAction = 1 << 3,						// 이동하지 않는 모든 움직임
			//PlayerMove = 1 << 4,						// 이동하는 움직임
			//PlayerMoveAndAction = (1 << 3) | (1 << 4),	// action 과 move 가 같이 일어날 수 있음
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
		EResult VillageChangedProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);		// current village info
		EResult VillageActionProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);
		EResult GoBackToCharacterSelectProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession);		//user id
	};
}


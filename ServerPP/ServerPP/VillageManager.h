#pragma once
namespace Village
{

	class VillageManager : public MyBase::Singleton<VillageManager>
	{
		friend class Singleton;
	public:
		enum class EProtocol : ProtocolSize_t
		{
			None = 0,

			VillageInit = 1 << 0,						// village 정보 전송
			EnterVillage = 1 << 1,						// village 입장
			ExitVillage = 1 << 2,						// village 퇴장
			PlayerAction = 1 << 3,						// 이동하지 않는 모든 움직임
			PlayerMove = 1 << 4,						// 이동하는 움직임
			PlayerMoveAndAction = (1 << 3) | (1 << 4),	// action 과 move 가 같이 일어날 수 있음
		};
		enum class EResult : ResultSize_t
		{
			None,
		};

		struct ResultData
		{
			EProtocol protocol;
			EResult result;
			NetBase::OutputMemoryStreamPtr outpStream;

			ResultData() : protocol(), result(), outpStream(nullptr) {}
		};
	protected:
		VillageManager() {}
		std::vector<VillageInfoBasePtr> m_villageInfo_vec;
	public:
		~VillageManager() {	}
		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;
	public:
		ResultData StateChangedProcess(uint64_t);	// village id
		ResultData VillageChangedProcess(uint64_t);	// village id
		ResultData VillageActionProcess(VillageInfoBasePtr, SectorPtr, PlayerInfoPtr);
		ResultData GoBackToCharacterSelectProcess(uint64_t);	//user id
	};
}


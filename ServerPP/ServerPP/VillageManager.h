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

			VillageInit,
			EnterVillage,
			ExitVillage,
			PlayerAction,
			PlayerMove,
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
		ResultData VillageActionProcess(const VillageInfoBase);
		ResultData GoBackToCharacterSelectProcess(uint64_t);	//user id
	};
}


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

			VillageInit = 1 << 0,						// village ���� ����
			EnterVillage = 1 << 1,						// village ����
			ExitVillage = 1 << 2,						// village ����
			PlayerAction = 1 << 3,						// �̵����� �ʴ� ��� ������
			PlayerMove = 1 << 4,						// �̵��ϴ� ������
			PlayerMoveAndAction = (1 << 3) | (1 << 4),	// action �� move �� ���� �Ͼ �� ����
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


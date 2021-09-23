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

			//VillageInit = 1 << 0,						// village ���� ����
			EnterVillage = 1 << 1,						// village ����
			ExitVillage = 1 << 2,						// village ����

			// 20 �������� ESectorProtocol ���� ���
			//PlayerAction = 1 << 3,						// �̵����� �ʴ� ��� ������
			//PlayerMove = 1 << 4,						// �̵��ϴ� ������
			//PlayerMoveAndAction = (1 << 3) | (1 << 4),	// action �� move �� ���� �Ͼ �� ����
		};
		enum class EResult : ResultSize_t
		{
			None,

			
		};

	
	protected:
		VillageManager() {}
		// db �κ��� �ҷ��� ���� container
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


#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Village::VillageManager);

namespace Village
{
	bool VillageManager::Initialize(LPVOID) noexcept
	{
#ifdef  __DEBUG
		printf("\n----- VillageManager::Initialize() -----\n");
#endif //  __DEBUG

		// TODO : SQL ���� village ������ ����������...
		// �ӽ� ����
		m_villageInfo_map.insert({ 1,VillageInfoBase::Create<FirstVillageInfo>() });

#ifdef  __DEBUG
		printf("----- VillageManager::Initialize() Complete -----\n");
#endif //  __DEBUG
		return true;
	}
	void VillageManager::Finalize() noexcept
	{
#ifdef  __DEBUG
		printf("\n----- VillageManager::Finalize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- VillageManager::Finalize() Complete -----\n");
#endif //  __DEBUG
	}

	VillageInfoBasePtr VillageManager::GetVillageInfo(uint32_t inVillageID)
	{
		return m_villageInfo_map[inVillageID];
	}


	VillageManager::EResult VillageManager::StateChangedProcess(IOCPSessionPtr inpSession)
	{
		EResult result = EResult::None;

		{	// player �� �� village info ���� �� village info ����
			uint32_t start_village_id = 1U;
			inpSession->SetVillageInfo(m_villageInfo_map[start_village_id]);

			EProtocol protocol = EProtocol::EnterVillage;

			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// write to stream
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
			write_size += NetBase::WriteToBinStream(stream, start_village_id);
			// write end
#ifdef __DEBUG	
			printf("VillageState village changed process  write to stream : %dbytes\n", write_size);
#endif
			// send data to CLIENT
			inpSession->Send(stream);
		}

		{	// player �� Village �� �����Ű��
			auto village_info = inpSession->GetVillageInfo();
			auto player_info = inpSession->GetPlayerInfo();
			// player �� ������ ��Ͻ�Ű��
			// ���ο��� Sector manager �� ���� ���� �۾����� �Ϸ�
			village_info->RegistEnterPlayerObj(player_info);
		}

		return result;
	}

	VillageManager::EResult VillageManager::VillageEnterProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		uint32_t start_village_id = 1U;

		// client �� ���� ������ ������ ���� ��������
		NetBase::ReadFromBinStream(inpStream, start_village_id);


		{	// init info
			inpSession->SetVillageInfo(m_villageInfo_map[start_village_id]);

			EProtocol protocol = EProtocol::EnterVillage;
			EResult result = EResult::None;

			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// write to stream
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
			write_size += NetBase::WriteToBinStream(stream, start_village_id);
			// write end
#ifdef __DEBUG	
			printf("VillageManager village changed process  write to stream : %dbytes\n", write_size);
#endif
			// send data to CLIENT
			inpSession->Send(stream);
		}

		{	// player �� Village �� �����Ű��
			auto village_info = inpSession->GetVillageInfo();
			auto player_info = inpSession->GetPlayerInfo();
			// player �� ������ ��Ͻ�Ű��
			// ���ο��� Sector manager �� ���� ���� �۾����� �Ϸ�
			village_info->RegistEnterPlayerObj(player_info);
		}

		return EResult();
	}
	VillageManager::EResult VillageManager::VillageExitProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		return EResult();
	}
	VillageManager::EResult VillageManager::VillageActionAndMoveProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		return EResult();
	}
	VillageManager::EResult VillageManager::VillageActionProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		return EResult();
	}
	VillageManager::EResult VillageManager::VillageMoveProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{	// client �� ���� move packet�� ������� ���
		auto pPlayer_info = inpSession->GetPlayerInfo();
		auto pVillage_info = inpSession->GetVillageInfo();

		// ���� �Ŵ����� ����
		SectorManager::sInstance->MoveProcess(
			inpStream, 
			pVillage_info->m_sectors, 
			pPlayer_info->GetSector(),
			pPlayer_info);

		return EResult();
	}
	VillageManager::EResult VillageManager::GoBackToCharacterSelectProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		return EResult();
	}
}
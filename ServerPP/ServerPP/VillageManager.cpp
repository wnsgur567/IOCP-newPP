#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Village::VillageManager);

namespace Village
{
	bool VillageManager::Initialize(LPVOID) noexcept
	{
#ifdef  __DEBUG
		printf("\n----- VillageManager::Initialize() -----\n");
#endif //  __DEBUG

		// TODO : SQL 에서 village 정보를 가져오도록...
		// 임시 생성
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
		
		{	// player 가 들어갈 village info 셋팅 및 village info 전송
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

		{	// player 를 Village 에 입장시키기
			auto village_info = inpSession->GetVillageInfo();
			auto player_info = inpSession->GetPlayerInfo();
			// player 를 마을에 등록시키고
			// 내부에서 Sector manager 를 통해 관련 작업까지 완료
			village_info->RegistEnterPlayerObj(player_info);
		}

		return result;
	}

	VillageManager::EResult VillageManager::VillageChangedProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		uint32_t start_village_id = 1U;

		// client 가 새로 입장한 마을의 정보 가져오기
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

			return result;
		}

		{	// enter info

			//// 현재 village 에 player 를 등록
			//inpVillageInfo->RegistEnterPlayerObj(inpPlayer);

			//// sector manager 에게 sectors 를 넘기고 stream 받아오기

			//EProtocol protocol = EProtocol::EnterVillage;
			//auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			//int write_size = 0;

		}

		return EResult();
	}
	VillageManager::EResult VillageManager::VillageActionProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		return EResult();
	}
	VillageManager::EResult VillageManager::GoBackToCharacterSelectProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		return EResult();
	}
}
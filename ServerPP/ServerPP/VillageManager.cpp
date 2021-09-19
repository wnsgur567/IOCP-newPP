#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Village::VillageManager);

namespace Village
{
	bool VillageManager::Initialize(LPVOID) noexcept
	{
		// SQL 에서 정보를 가져오도록...

		// 임시 생성
		m_villageInfo_vec.push_back(VillageInfoBase::Create<FirstVillageInfo>());
		return false;
	}
	void VillageManager::Finalize() noexcept
	{
	}
	VillageManager::ResultData VillageManager::StateChangedProcess(uint64_t inVillageID)
	{
		ResultData retData;

		retData.protocol = EProtocol::VillageInit;
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)retData.protocol);
		//write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)retData.result);
		write_size += NetBase::WriteToBinStream(stream, inVillageID);
#ifdef __DEBUG	
		printf("CharacterState write to stream : %dbytes\n", write_size);
#endif
		retData.outpStream = stream;
		return retData;
	}

	VillageManager::ResultData VillageManager::VillageChangedProcess(uint64_t inVillageID)
	{
		ResultData retData;

		retData.protocol = EProtocol::VillageInit;
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)retData.protocol);
		//write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)retData.result);
		write_size += NetBase::WriteToBinStream(stream, inVillageID);
#ifdef __DEBUG	
		printf("CharacterState write to stream : %dbytes\n", write_size);
#endif
		retData.outpStream = stream;
		return retData;
	}
	VillageManager::ResultData VillageManager::VillageActionProcess(VillageInfoBasePtr inVillageInfo, SectorPtr inCurrentSector, PlayerInfoPtr inPlayerInfo)
	{
		ResultData retData;



		return retData;
	}
	VillageManager::ResultData VillageManager::GoBackToCharacterSelectProcess(uint64_t)
	{
		return ResultData();
	}
}
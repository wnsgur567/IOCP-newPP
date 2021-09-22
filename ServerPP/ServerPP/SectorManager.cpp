#include "IOCPNet_RootHeader.h"

bool SectorManager::Initialize(LPVOID) noexcept
{
	return true;
}

void SectorManager::Finalize() noexcept
{
}

void SectorManager::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{
	if ((ProtocolSize_t)EProtocol::PlayerAction & inOrigin)
	{
		if ((ProtocolSize_t)EProtocol::PlayerMove & inOrigin)
		{
			outProtocol = EProtocol::PlayerMoveAndAction;
			return;
		}
		outProtocol = EProtocol::PlayerAction;
		return;
	}
	if ((ProtocolSize_t)EProtocol::PlayerMove & inOrigin)
	{
		outProtocol = EProtocol::PlayerMove;
		return;
	}
}

SectorManager::EResult SectorManager::ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer)
{	// 내부에서만 행동하니 섹터 바뀔 일 없음
	return {};
}

SectorManager::EResult SectorManager::MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// 섹터 외부로 이동 가능성 있음
	
	// stream 으로부터 현재 player 의 position 값 가져오기

	auto player_pos = inpPlayer->GetPosition();
	// 기존에 있던 섹터를 기준으로 판별
	auto dir = inCurrent_sector->GetRelativeDirInSector(player_pos);

	switch (dir)
	{
	case Sector::EDirection::Middle:
	{	// 섹터 이동하지 않음

	}
	break;
	// 섹터 이동함
	// 섹터 갱신 필요
	case Sector::EDirection::LeftUpper:
		break;
	case Sector::EDirection::MiddleUpper:
		break;
	case Sector::EDirection::RightUpper:
		break;
	case Sector::EDirection::LeftMiddle:
		break;
	case Sector::EDirection::RightMiddle:
		break;
	case Sector::EDirection::LeftLower:
		break;
	case Sector::EDirection::MiddleLower:
		break;
	case Sector::EDirection::RightLower:
		break;
	default:
		break;
	}

	return {};
}

SectorManager::EResult SectorManager::MoveAndActionProcess(std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// 섹터 외부로 이동 가능성 있음
	return {};
}


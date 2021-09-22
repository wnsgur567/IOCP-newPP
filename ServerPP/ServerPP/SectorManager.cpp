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
{	// ���ο����� �ൿ�ϴ� ���� �ٲ� �� ����
	return {};
}

SectorManager::EResult SectorManager::MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// ���� �ܺη� �̵� ���ɼ� ����
	
	// stream ���κ��� ���� player �� position �� ��������

	auto player_pos = inpPlayer->GetPosition();
	// ������ �ִ� ���͸� �������� �Ǻ�
	auto dir = inCurrent_sector->GetRelativeDirInSector(player_pos);

	switch (dir)
	{
	case Sector::EDirection::Middle:
	{	// ���� �̵����� ����

	}
	break;
	// ���� �̵���
	// ���� ���� �ʿ�
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
{	// ���� �ܺη� �̵� ���ɼ� ����
	return {};
}


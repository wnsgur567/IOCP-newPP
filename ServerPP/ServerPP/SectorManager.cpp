#include "IOCPNet_RootHeader.h"

bool SectorManager::Initialize(LPVOID) noexcept
{
	return true;
}

void SectorManager::Finalize() noexcept
{
}

void SectorManager::MoveOtherSector(std::vector<std::vector<SectorPtr>>& inoutSectors, SectorPtr inCurrent_sector, EDirection inDir)
{

}

void SectorManager::ActionInSector(SectorPtr inCurrentSector)
{

}

#pragma once
class SectorManager : public MyBase::Singleton<SectorManager>
{
	friend class Singleton;
	using EDirection = Sector::EDirection;
protected:
	SectorManager() {}
public:
	~SectorManager()
	{

	}
	bool Initialize(LPVOID) noexcept override;
	void Finalize() noexcept override;
public:
	void MoveOtherSector(std::vector<std::vector<SectorPtr>>& inoutSectors, SectorPtr inCurrent_sector, EDirection inDir);
	void ActionInSector(SectorPtr inCurrentSector);
};


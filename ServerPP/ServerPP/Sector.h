#pragma once

class Sector;
using SectorPtr = std::shared_ptr<Sector>;
class Sector : public std::enable_shared_from_this<Sector>
{
	friend class SectorManager;
public:
	enum class EDirection
	{
		LeftUpper,
		MiddleUpper,
		RightUpper,

		LeftMiddle,
		Middle,
		RightMiddle,

		LeftLower,
		MiddleLower,
		RightLower
	};

private:
	bool IsAccesible;
	uint32_t m_sector_id;
	Vector2Int m_left_top_pixel_position;	// 현재 맵을 기준으로 현제 섹터의 left top point 의 위치
	Vector2Int m_size;						// width height
	Vector2Int m_grid_position;				// 현재 맵을 기준으로 2차원 관리 벡터에서의 grid pos

	//std::vector<SectorPtr> m_nearSector_vec;	
	std::vector<std::vector<SectorPtr>> m_nearSector_vec;	// 좌상단부터 3*3
	// NetGameObject id to info
	std::unordered_map<uint64_t, PlayerInfoPtr> m_player_map;	
public:
	Sector()
		: IsAccesible(true), m_sector_id(0), m_left_top_pixel_position(), m_size(),
		m_grid_position(), m_nearSector_vec() {}
	void Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate = true);		// 위치값 설정 
	void SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors);	// after initilaized
	void Finalize();	// release

protected:
	EDirection GetRelativeDirInSector(const Vector3&);
	Vector2Int GetPixelPos() const { return m_left_top_pixel_position; }

	bool IsInSector(const Vector2& vec);
	bool IsInSector(const Vector2Int& vec);
	void GetNearPlayerInfos(std::vector<PlayerInfoPtr>& outVec);	
public:	
	virtual void MoveInSection(PlayerInfoPtr);				// section 내부에서 이동 시
	virtual void EnterSection(PlayerInfoPtr,EDirection);	// 현재 section 에서 타 section으로 이동 시
	virtual void LeaveSection(PlayerInfoPtr, EDirection);	// 
	virtual void FirstEnterSection(PlayerInfoPtr);
	virtual void DisapearSection(PlayerInfoPtr);
	void SendAll(NetBase::OutputMemoryStreamPtr);
	void SendAllInSector(NetBase::OutputMemoryStreamPtr);
	void SendAllExceptOne(NetBase::OutputMemoryStreamPtr inpStream, uint64_t);
};
#pragma once

class Sector;
using SectorPtr = std::shared_ptr<Sector>;
class Sector
{
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

	std::vector<SectorPtr> m_nearSector_vec;	// 좌상단부터 8방위

	// NetGameObject id to info
	std::unordered_map<uint64_t, PlayerInfoPtr> m_player_map;
	// NPC...
	// std::unordered_map<uint64_t , NpcInfoPtr> m_npc_map;
public:
	Sector()
		: IsAccesible(true), m_sector_id(0), m_left_top_pixel_position(), m_size(),
		m_grid_position(), m_nearSector_vec() {}
	void Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate = true);		// 위치값 설정 
	void SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors);	// after initilaized
	void Finalize();	// release

public:
	EDirection GetRelativeDirInSector(const Vector3&);
	Vector2Int GetPixelPos() const { return m_left_top_pixel_position; }
public:
	bool IsInSector(const Vector2& vec);
	bool IsInSector(const Vector2Int& vec);
	void PlayerMove(PlayerInfoPtr);	
	void GetNearPlayerInfos(std::vector<PlayerInfoPtr>& outVec);

	virtual void EnterSection(PlayerInfoPtr);
	virtual void LeaveSection(PlayerInfoPtr);
	// 현재 섹터에 존재하는 모든 player 에게 정보를 보내기
	//void SendEnterPacket();
	//void SendLeavePacket();
	//void SendPacket();
};
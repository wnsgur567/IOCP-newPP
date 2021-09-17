#pragma once

class Sector;
using SectorPtr = std::shared_ptr<Sector>;
class Sector
{
private:
	uint32_t m_sector_id;
	Vector2 m_left_top_pixcel_position;		// 현재 맵을 기준으로 현제 섹터의 left top point 의 위치
	Vector2Int m_grid_position;				// 현재 맵을 기준으로 2차원 관리 벡터에서의 grid pos
	
	std::vector<SectorPtr> m_nearSector_vec;	// 좌상단부터 8방위

	std::vector<PlayerInfoPtr> m_player_vec;
	// NPC...
public:
	// 현재 섹터에 존재하는 모든 player 에게 정보를 보내기
	void SendAll();
	
};



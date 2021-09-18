#pragma once

class Sector;
using SectorPtr = std::shared_ptr<Sector>;
class Sector
{
private:
	bool IsAccesible;
	uint32_t m_sector_id;
	Vector2Int m_left_top_pixcel_position;	// 현재 맵을 기준으로 현제 섹터의 left top point 의 위치
	Vector2Int m_size;						// width height
	Vector2Int m_grid_position;				// 현재 맵을 기준으로 2차원 관리 벡터에서의 grid pos
	
	std::vector<SectorPtr> m_nearSector_vec;	// 좌상단부터 8방위

	// NetGameObject id to info
	std::unordered_map<uint64_t, PlayerInfoPtr> m_player_map;
	// NPC...
private:
	

public:
	void Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate = true);		// 위치값 설정 
	void SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors);	// after initilaized
	void Finalize();	// release

	void EnterSection(PlayerInfoPtr);
	void LeaveSection(PlayerInfoPtr);
	// 현재 섹터에 존재하는 모든 player 에게 정보를 보내기
	void SendEnterPacket();
	void SendLeavePacket();
	void SendPacket();
};
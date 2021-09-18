#pragma once

class Sector;
using SectorPtr = std::shared_ptr<Sector>;
class Sector
{
private:
	bool IsAccesible;
	uint32_t m_sector_id;
	Vector2Int m_left_top_pixcel_position;	// ���� ���� �������� ���� ������ left top point �� ��ġ
	Vector2Int m_size;						// width height
	Vector2Int m_grid_position;				// ���� ���� �������� 2���� ���� ���Ϳ����� grid pos
	
	std::vector<SectorPtr> m_nearSector_vec;	// �»�ܺ��� 8����

	// NetGameObject id to info
	std::unordered_map<uint64_t, PlayerInfoPtr> m_player_map;
	// NPC...
private:
	

public:
	void Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate = true);		// ��ġ�� ���� 
	void SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors);	// after initilaized
	void Finalize();	// release

	void EnterSection(PlayerInfoPtr);
	void LeaveSection(PlayerInfoPtr);
	// ���� ���Ϳ� �����ϴ� ��� player ���� ������ ������
	void SendEnterPacket();
	void SendLeavePacket();
	void SendPacket();
};
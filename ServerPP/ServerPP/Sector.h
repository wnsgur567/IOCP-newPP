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

	std::vector<PlayerInfoPtr> m_player_vec;
	// NPC...
public:
	void Initialize();	// ��ġ�� ���� , �ֺ� ���� �˻�
	void Finalize();	// release
	// ���� ���Ϳ� �����ϴ� ��� player ���� ������ ������
	void SendAll();
	
};



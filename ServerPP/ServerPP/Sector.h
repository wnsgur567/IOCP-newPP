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
	Vector2Int m_left_top_pixcel_position;	// ���� ���� �������� ���� ������ left top point �� ��ġ
	Vector2Int m_size;						// width height
	Vector2Int m_grid_position;				// ���� ���� �������� 2���� ���� ���Ϳ����� grid pos

	std::vector<SectorPtr> m_nearSector_vec;	// �»�ܺ��� 8����

	// NetGameObject id to info
	std::unordered_map<uint64_t, PlayerInfoPtr> m_player_map;
	// NPC...
private:
	EDirection GetRelativePosInSector(const Vector3&);

public:
	Sector()
		: IsAccesible(true), m_sector_id(0), m_left_top_pixcel_position(), m_size(),
		m_grid_position(), m_nearSector_vec() {}
	void Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate = true);		// ��ġ�� ���� 
	void SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors);	// after initilaized
	void Finalize();	// release


	void PlayerMove(PlayerInfoPtr);
	void PlayerAction(PlayerInfoPtr);

	virtual void EnterSection(PlayerInfoPtr);
	virtual void LeaveSection(PlayerInfoPtr);
	// ���� ���Ϳ� �����ϴ� ��� player ���� ������ ������
	//void SendEnterPacket();
	//void SendLeavePacket();
	//void SendPacket();
};
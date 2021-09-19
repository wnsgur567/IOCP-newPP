#include "IOCPNet_RootHeader.h"

// 1 2 3
// 4 5 6
// 7 8 9
Sector::EDirection Sector::GetRelativePosInSector(const Vector3& pos)
{	// unity 기준 y가 점프
	// x check

	int x_flag = 0;
	int x_left = m_left_top_pixcel_position.x;
	if (pos.x < x_left)
		x_flag = -1;
	else if (x_left + m_size.x < pos.x)
		x_flag = 1;

	// z check
	int z_flag = 0;
	int z_top = m_left_top_pixcel_position.y;
	if (pos.z < z_top)
		z_flag = -1;
	else if (z_top + m_size.y < pos.z)
		z_flag = 1;


	if (x_flag == -1 && z_flag == -1)
		return EDirection::LeftUpper;
	if (x_flag == 0 && z_flag == -1)
		return EDirection::MiddleUpper;
	if (x_flag == 1 && z_flag == -1)
		return EDirection::RightUpper;
	if (x_flag == -1 && z_flag == 0)
		return EDirection::LeftMiddle;
	if (x_flag == 0 && z_flag == 0)
		return EDirection::Middle;
	if (x_flag == 1 && z_flag == 0)
		return EDirection::RightMiddle;
	if (x_flag == -1 && z_flag == 1)
		return EDirection::LeftLower;
	if (x_flag == 0 && z_flag == 1)
		return EDirection::MiddleLower;
	//if (x_flag == 1 && z_flag == 1)
	return EDirection::RightLower;
}

void Sector::Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate)
{
	m_left_top_pixcel_position = pos;
	m_grid_position = grid;
	m_size = size;
	IsAccesible = activate;
}

// after initilaized
void Sector::SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors)
{
	Vector2Int offset[3][3] = {
		{{-1,-1},{0,-1},{1,-1}},
		{{-1,0},{0,0},{1,0}},
		{{-1,1},{0,1},{1,1}}
	};

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			int row = m_grid_position.x + offset[i][j].x;
			int col = m_grid_position.y + offset[i][j].y;
			if (row < 0 || col < 0)
				continue;
			if (sectors[row][col]->IsAccesible)
				m_nearSector_vec.push_back(sectors[row][col]);
		}
	}
}

void Sector::Finalize()
{
}

void Sector::PlayerMove(PlayerInfoPtr inpPlayer)
{
	auto pos = inpPlayer->GetPosition();
	auto dir = GetRelativePosInSector(pos);

	switch (dir)
	{
	case Sector::EDirection::Middle:
		PlayerAction(inpPlayer);
		break;

	case Sector::EDirection::LeftUpper:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::MiddleUpper:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::RightUpper:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::LeftMiddle:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::RightMiddle:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::LeftLower:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::MiddleLower:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::RightLower:
		LeaveSection(inpPlayer);
		break;
	default:
		// 예외처리
		break;
	}
}

void Sector::PlayerAction(PlayerInfoPtr inpPlayer)
{

}

void Sector::EnterSection(PlayerInfoPtr inpPlayer)
{
	m_player_map.insert({ inpPlayer->GetNetID(), inpPlayer });
}

void Sector::LeaveSection(PlayerInfoPtr inpPlayer)
{
	m_player_map.erase(inpPlayer->GetNetID());
}


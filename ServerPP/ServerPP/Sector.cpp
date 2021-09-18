#include "IOCPNet_RootHeader.h"

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
			m_nearSector_vec.push_back(sectors[row][col]);
		}
	}
}

void Sector::Finalize()
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


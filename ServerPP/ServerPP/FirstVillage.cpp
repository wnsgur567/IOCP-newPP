#include "IOCPNet_RootHeader.h"

namespace Village
{
	void FirstVillageInfo::Initialize()
	{
		// sql 에서 불러오면 Manager 에 저장되 있는 정보를 바탕으로 초기화

		m_village_name = L"TestVillage";
		m_player_view_range = 200;
		m_grid_size.x = 8;
		m_grid_size.y = 3;
		m_pixel_size.x = 180;
		m_pixel_size.y = 180;

		m_sectors.resize(m_grid_size.x);
		for (size_t i = 0; i < m_grid_size.x; i++)
		{
			m_sectors[i].reserve(m_grid_size.y);
			for (size_t j = 0; j < m_grid_size.y; j++)
			{
				SectorPtr pSector = std::make_shared<Sector>();
				pSector->Initialize(
					{ static_cast<int>(i) * m_pixel_size.x,static_cast<int>(j) * m_pixel_size.y },
					{ static_cast<int>(i),static_cast<int>(j) },
					m_pixel_size);
				m_sectors[i].push_back(pSector);
			}
		}
	}


	void FirstVillageInfo::Finalize()
	{

	}
}


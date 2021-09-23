#include "IOCPNet_RootHeader.h"

namespace Village
{
	void FirstVillageInfo::Initialize()
	{
		// sql 에서 불러오면 Manager 에 저장되 있는 정보를 바탕으로 초기화

		// set this village infomation
		m_village_name = L"TestVillage";
		m_player_view_range = 200;
		m_grid_size.x = 8;
		m_grid_size.y = 3;
		m_sector_size.x = 224;
		m_sector_size.y = 140;
		m_pixel_size.x = 224 * 8;
		m_pixel_size.y = 140 * 3;

		// set sectors infomation
		m_sectors.resize(m_grid_size.x);
		for (size_t i = 0; i < m_grid_size.x; i++)
		{
			m_sectors[i].reserve(m_grid_size.y);
			for (size_t j = 0; j < m_grid_size.y; j++)
			{
				SectorPtr pSector = std::make_shared<Sector>();
				pSector->Initialize(
					{ static_cast<int>(i) * m_sector_size.x,static_cast<int>(j) * m_sector_size.y },
					{ static_cast<int>(i),static_cast<int>(j) },
					m_sector_size);
				m_sectors[i].push_back(pSector);
			}
		}

		// set sector's nearVecors
		for (auto& item : m_sectors)
		{
			for (auto& inner : item)
			{
				inner->SetNearSector(m_sectors);
			}
		}

		m_enter_pos = std::make_shared<VillageEnterPositon>();
		m_enter_pos->m_grid_position.x = 0;
		m_enter_pos->m_grid_position.y = 1;
		m_enter_pos->m_local_position.x = 20.f;
		m_enter_pos->m_local_position.y = 90.f;
	}


	void FirstVillageInfo::Finalize()
	{

	}
	void FirstVillageInfo::OnPlayerRegisted(PlayerInfoPtr pInfo)
	{
		auto session = pInfo->GetSession();
		auto pCurrent_sector = m_sectors[m_enter_pos->m_grid_position.x][m_enter_pos->m_grid_position.y];
		// 해당 위치 섹터에 배치 및 관련 정보를 전송하도록
		SectorManager::sInstance->FirstEnterProcess(pCurrent_sector, m_enter_pos->m_local_position, pInfo, session);
	}
	void FirstVillageInfo::BeforePlayerDelete(PlayerInfoPtr pInfo)
	{
	}
}


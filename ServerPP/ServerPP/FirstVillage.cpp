#include "IOCPNet_RootHeader.h"

namespace Village
{
	void FirstVillageInfo::Initialize()
	{
		// sql ���� �ҷ����� Manager �� ����� �ִ� ������ �������� �ʱ�ȭ

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
		auto pCurrent_sector = m_sectors[m_enter_pos->m_grid_position.x][m_enter_pos->m_grid_position.y];
		// player ��ġ ����
		Vector2Int sector_pos = pCurrent_sector->GetPixelPos();
		float x_pos = static_cast<float>(sector_pos.x) + m_enter_pos->m_local_position.x;
		float y_pos = static_cast<float>(sector_pos.y) + m_enter_pos->m_local_position.y;
		pInfo->SetPosition(x_pos, y_pos, 0.f);

		// ���Ϳ� player ���
		pCurrent_sector->EnterSection(pInfo);
		// player�� sector ���
		pInfo->SetSector(pCurrent_sector);
	}
	void FirstVillageInfo::BeforePlayerDelete(PlayerInfoPtr pInfo)
	{
	}
}


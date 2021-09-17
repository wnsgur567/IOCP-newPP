#pragma once

// village super
class VillageInfoBase
{
	uint32_t m_village_id;
	std::wstring m_village_name;
	
	uint32_t m_player_view_range;	// 현재 맵에서 player 의 시야 범위 , radius
	Vector2 m_pixel_size;			// 현재 맵의 픽셀 기준 크기

	std::vector<std::vector<Sector>> m_sectors;
};
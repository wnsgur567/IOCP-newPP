#pragma once

// village super
class VillageInfoBase
{
	uint32_t m_village_id;
	std::wstring m_village_name;
	
	uint32_t m_player_view_range;	// ���� �ʿ��� player �� �þ� ���� , radius
	Vector2 m_pixel_size;			// ���� ���� �ȼ� ���� ũ��

	std::vector<std::vector<Sector>> m_sectors;
};
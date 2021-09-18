#pragma once

namespace Village
{
	class VillageInfoBase;
	using VillageInfoBasePtr = std::shared_ptr<VillageInfoBase>;
	// village super
	class VillageInfoBase
	{
	protected:
		uint32_t m_village_id;
		std::wstring m_village_name;

		uint32_t m_player_view_range;	// 현재 맵에서 player 의 시야 범위 , radius
		Vector2Int m_grid_size;
		Vector2Int m_pixel_size;		// 현재 맵의 픽셀 기준 크기

		std::vector<std::vector<SectorPtr>> m_sectors;

		VillageInfoBase() :
			m_village_id(0), m_village_name(), m_player_view_range(0), m_grid_size(), m_pixel_size() {}
	public:
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
		template <typename _Ty>
		static VillageInfoBasePtr Create();
	};
	template<typename _Ty>
	inline VillageInfoBasePtr VillageInfoBase::Create()
	{
		_Ty* derived_created = new typename std::enable_if< std::is_base_of<VillageInfoBase, _Ty>::value, _Ty>::type();
		VillageInfoBasePtr retPtr;
		retPtr.reset(derived_created);
		retPtr->Initialize();
		return retPtr;
	}
}
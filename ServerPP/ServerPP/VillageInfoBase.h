#pragma once

namespace Village
{
	// village ���� ����� player pos ������ ���� ����
	struct VillageEnterPositon
	{
	public:
		Vector2Int m_grid_position;
		Vector2 m_local_position;
	};
	using VillageEnterPositonPtr = std::shared_ptr<VillageEnterPositon>;



	class VillageInfoBase;
	using VillageInfoBasePtr = std::shared_ptr<VillageInfoBase>;
	// village super
	class VillageInfoBase : public ISerializable
	{
		friend class VillageManager;
	protected:
		uint32_t m_village_id;
		std::wstring m_village_name;

		uint32_t m_player_view_range;	// ���� �ʿ��� player �� �þ� ���� , radius
		Vector2Int m_grid_size;
		Vector2Int m_sector_size;		// sector �� ũ��
		Vector2Int m_pixel_size;		// ���� ���� �ȼ� ���� ũ��

		std::vector<std::vector<SectorPtr>> m_sectors;		// ���ҵ� 2���� ����
		VillageEnterPositonPtr m_enter_pos;					// ���� ���� �� ���� ��ġ ����
	protected:
		std::unordered_map<uint64_t, PlayerInfoPtr> m_IdToPlayer_map;
	public:
		VillageInfoBase() :
			m_village_id(0), m_village_name(), m_player_view_range(0), m_grid_size(), m_sector_size(), m_pixel_size(),
			m_IdToPlayer_map() {}
		~VillageInfoBase() { }
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

		uint32_t GetVillageID() const { return m_village_id; }

	public:
		void RegistEnterPlayerObj(PlayerInfoPtr);	
		void DeleteExitPlayerObj(uint32_t);	

		virtual void OnPlayerRegisted(PlayerInfoPtr) = 0;		// player �� ���� village �� ��ϵ� ���� ó���� ��
		virtual void BeforePlayerDelete(PlayerInfoPtr) = 0;		// player �� ���� village ���� ���ܵǱ� �� ó���� ��
	public:
		virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
		virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;

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
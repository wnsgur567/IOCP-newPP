#pragma once

namespace Village
{
	class FirstVillageInfo : public Village::VillageInfoBase
	{
		friend VillageInfoBase;
	protected:
		FirstVillageInfo() {}
	public:
		~FirstVillageInfo() {}
		virtual void Initialize() override;
		virtual void Finalize() override;

		// VillageInfoBase��(��) ���� ��ӵ�
		virtual void OnPlayerRegisted(PlayerInfoPtr) override;
		virtual void BeforePlayerDelete(PlayerInfoPtr) override;
	};

	using FirstVillagePtr = std::shared_ptr<FirstVillageInfo>;
}
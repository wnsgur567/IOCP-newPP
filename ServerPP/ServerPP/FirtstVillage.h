#pragma once

namespace Village
{
	class FirstVillageInfo : public Village::VillageInfoBase
	{
		friend VillageInfoBase;
	protected:
		FirstVillageInfo() {}
	public:
		virtual void Initialize() override;
		virtual void Finalize() override;
	};

	using FirstVillagePtr = std::shared_ptr<FirstVillageInfo>;
}
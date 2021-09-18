#pragma once
namespace Village
{

	class VillageManager
	{
	public:
		enum class EProtocol : ProtocolSize_t
		{
			None = 0,

			EnterVillage,
			ExitVillage,
			PlayerAction,
			PlayerMove,
		};
		enum class EResult : ResultSize_t
		{
			None,
		};
		enum class EVillageType
		{

		};
	};
}


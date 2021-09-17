#pragma once

namespace CharacterSelect
{

	class CharacterSelectManager : public MyBase::Singleton<CharacterSelectManager>
	{
		friend class Singleton;
	public:
		enum class EProtocol : ProtocolSize_t
		{
			None = 0,

			AllCharacterInfo = 1 << 0,
			CharacterSelect = 1 << 1,
			SignOut = 1 << 2,
		};

		enum class EResult : ResultSize_t
		{
			None = 0,

			NoData = 1 << 0,
			CharaterInfos = 1 << 1,
			Success_CharacterSelect = 1 << 2,
			UndefinedCharacter = 1 << 3,
		};
		struct ResultMSG
		{
		public:
			static const wchar_t* NoCharacterMsg;
			static const wchar_t* CharacterInfosMsg;
			static const wchar_t* CharacterSelectSuccessMsg;
			static const wchar_t* UndefinedCharacterMsg;
		};
		struct ResultData
		{
			EProtocol protocol;
			EResult result;
			NetBase::OutputMemoryStreamPtr outpStream;		
		};
	protected:
		CharacterSelectManager() {}
	public:
		~CharacterSelectManager() {}
		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;
	private:
		bool IsExist(uint64_t character_id) const;
		void LoadInfo(uint64_t character_id, CharacterInfoPtr& outInfo);
		void LoadInfos(uint64_t user_id, std::vector<CharacterInfoPtr>& outVec);
		void SaveInfo(const CharacterInfoPtr);
		void DeleteInfo(const CharacterInfoPtr inInfo);
	public:

	public:
		ResultData StateInitializeProcess(uint64_t);
		ResultData CharacterSelectProcess(const CharacterInfoPtr inInfo);
		ResultData MakeNewCharacterProcess(const CharacterInfoPtr inInfo);
		ResultData SignOutProcess(const CharacterInfoPtr inInfo);





	};
}
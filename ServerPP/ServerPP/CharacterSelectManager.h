#pragma once

class CharacterSelectManager : public MyBase::Singleton<CharacterSelectManager>
{
	friend class Singleton;
public:
	enum class EProtocol : ProtocolSize_t
	{
		None = 0,

		CharacterSelect = 1 << 0,
		SignOut = 1 << 1,
	};

	enum class EResult : ResultSize_t
	{
		None = 0,

		Success_CharacterSelect = 1 << 0,
	};
	struct ResultMSG
	{
	public:
		static const wchar_t* CharacterSelectSuccessMsg;
	};
	struct ResultData
	{
		EProtocol protocol;
		EResult result;
		const wchar_t* msg;
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
	ResultData CharacterSelectProcess(const CharacterInfoPtr inInfo);
	ResultData SignOutProcess(const CharacterInfoPtr inInfo);
};
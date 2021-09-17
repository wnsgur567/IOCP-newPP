#pragma once

class CharacterInfo : public ISerializable
{
public:
	uint64_t character_id;		// character 고유 id
	uint64_t user_id;			// character 를 소유하고 있는 user의 고유 id
	uint32_t character_type;	// character 종류 (직업)
	std::wstring character_name;	// character 이름
public:
	CharacterInfo()
		:character_id(), user_id(), character_type(), character_name() {}
	void SetInfo(uint64_t id, uint64_t user_id, uint32_t char_type, std::wstring& char_name);
	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;

	bool operator==(const CharacterInfo& other)
	{
		if (character_id == other.character_id)
			return true;
		return false;
	}
};

using CharacterInfoPtr = std::shared_ptr<CharacterInfo>;
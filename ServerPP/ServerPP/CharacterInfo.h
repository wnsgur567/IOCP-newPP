#pragma once

class CharacterInfo : public ISerializable
{
public:
	uint64_t character_id;
	uint64_t user_id;
	uint32_t character_type;
	std::wstring character_name;
public:
	void SetInfo(uint64_t id, uint64_t user_id, uint32_t char_type, std::wstring& char_name);
	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};

using CharacterInfoPtr = std::shared_ptr<CharacterInfo>;
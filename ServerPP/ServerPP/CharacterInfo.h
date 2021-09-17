#pragma once

class CharacterInfo : public ISerializable
{
public:
	uint64_t character_id;		// character ���� id
	uint64_t user_id;			// character �� �����ϰ� �ִ� user�� ���� id
	uint32_t character_type;	// character ���� (����)
	std::wstring character_name;	// character �̸�
public:
	CharacterInfo()
		:character_id(), user_id(), character_type(), character_name() {}
	void SetInfo(uint64_t id, uint64_t user_id, uint32_t char_type, std::wstring& char_name);
	// ISerializable��(��) ���� ��ӵ�
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
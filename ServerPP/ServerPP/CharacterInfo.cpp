#include "IOCPNet_RootHeader.h"

void CharacterInfo::SetInfo(uint64_t id, uint64_t user_id, uint32_t char_type, std::wstring& char_name)
{
	this->character_id = id;
	this->user_id = user_id;
	this->character_type = char_type;
	this->character_name = char_name;
}

int CharacterInfo::Serialize(NetBase::OutputMemoryStreamPtr out_stream)
{
	int size = 0;
	WriteToStream(out_stream, character_id);
	WriteToStream(out_stream, user_id);
	WriteToStream(out_stream, character_type);
	WriteToStream(out_stream, character_name);
	return 0;
}

int CharacterInfo::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	int size = 0;
	ReadFromStream(in_stream, character_id);
	ReadFromStream(in_stream, user_id);
	ReadFromStream(in_stream, character_type);
	ReadFromStream(in_stream, character_name);
	return 0;
}

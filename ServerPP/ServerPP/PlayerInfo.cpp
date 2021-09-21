#include "IOCPNet_RootHeader.h"

PlayerInfo::PlayerInfo()
	: m_character_info(nullptr), m_posisition()
{
	m_character_info = std::make_shared<CharacterInfo>();
}

int PlayerInfo::Serialize(NetBase::OutputMemoryStreamPtr out_stream)
{
	int size = 0;
	return size;
}

int PlayerInfo::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	int size = 0;
	return size;
}

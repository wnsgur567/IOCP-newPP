#include "IOCPNet_RootHeader.h"

PlayerInfo::PlayerInfo()
	: NetGameObject(NetGameObject::ENetGameObjectType::Player),
	m_character_info(nullptr), m_posisition(), m_pOwnerSession()
{
	
}

int PlayerInfo::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
{ 
	int size = 0;
	size += NetGameObject::Serialize(out_stream);
	WriteToStream(out_stream, m_character_info);
	WriteToStream(out_stream, m_posisition);
	return size;
}

int PlayerInfo::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	int size = 0;
	size += NetGameObject::DeSerialize(in_stream);
	ReadFromStream(in_stream, m_character_info);	
	ReadFromStream(in_stream, m_posisition);
	return size;
}

void PlayerInfo::OnCreated()
{
	printf("created new PlayerInfo\n");
	m_character_info = std::make_shared<CharacterInfo>();
}

void PlayerInfo::BeforeDestroy()
{
	printf("Destroyed PlayerInfo\n");
}


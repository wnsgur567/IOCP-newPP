#include "IOCPNet_RootHeader.h"
#include<assert.h>


void PlayerPartyInfo::Initialize(uint32_t inPartyID, PlayerInfoPtr creater, std::wstring inParty_name, int inMax_capacity)
{
	m_player_vec.resize(MAX_CONSTRAINT);

	// ��Ƽ ���� �ʱ� ����
	m_party_id = inPartyID;
	m_name = inParty_name;
	m_max_playercount = inMax_capacity;
	m_cur_playercount = 1;
	m_owner_index = 0;
	m_player_vec[0] = creater;
	m_owner = creater;
}

// id + ��Ƽ �̸� + ���� �ο� �� + �ִ� �ο� �� + ��Ƽ�� index ��ȣ + ��Ƽ�� ����� ����
int PlayerPartyInfo::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
{
	int size = 0;
	NetBase::WriteToBinStream(out_stream, m_party_id);
	NetBase::WriteToBinStream(out_stream, m_name);
	NetBase::WriteToBinStream(out_stream, m_cur_playercount);
	NetBase::WriteToBinStream(out_stream, m_max_playercount);
	NetBase::WriteToBinStream(out_stream, m_owner_index);
	NetBase::WriteToBinStream(out_stream, m_player_vec);
	return 0;
}

// ... ���� �� ����
int PlayerPartyInfo::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{	
	static_assert(true, "do not use\n");
	return 0;
}

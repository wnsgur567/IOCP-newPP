#include "IOCPNet_RootHeader.h"
#include<assert.h>


void PlayerPartyInfo::Initialize(uint32_t inPartyID, PlayerInfoPtr creater, std::wstring inParty_name, int inMax_capacity)
{
	m_player_vec.resize(MAX_CONSTRAINT);

	// 파티 정보 초기 셋팅
	m_party_id = inPartyID;
	m_name = inParty_name;
	m_max_playercount = inMax_capacity;
	m_cur_playercount = 1;
	m_owner_index = 0;
	m_player_vec[0] = creater;
	m_owner = creater;
}

// id + 파티 이름 + 현재 인원 수 + 최대 인원 수 + 파티장 index 번호 + 파티원 모두의 정보
int PlayerPartyInfo::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
{
	int size = 0;
	size += NetBase::WriteToBinStream(out_stream, m_party_id);
	size += NetBase::WriteToBinStream(out_stream, m_name);
	size += NetBase::WriteToBinStream(out_stream, m_cur_playercount);
	size += NetBase::WriteToBinStream(out_stream, m_max_playercount);
	size += NetBase::WriteToBinStream(out_stream, m_owner_index);
	size += NetBase::WriteToBinStream(out_stream, m_player_vec);
	return size;
}

// ... 받을 일 없음
int PlayerPartyInfo::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	assert("do not use\n");
	return 0;
}

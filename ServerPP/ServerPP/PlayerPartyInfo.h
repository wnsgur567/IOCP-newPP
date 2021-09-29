#pragma once
class PlayerPartyInfo;
using PlayerPartyInfoPtr = std::shared_ptr<PlayerPartyInfo>;
class PlayerPartyInfo : public ISerializable
{
	friend class PlayerPartyManager;

	// 하나의 파티에 수용 가능한 최대 인원 수
	constexpr static int MAX_CONSTRAINT = 4;

	uint32_t m_party_id;		// 파티 고유 number
	std::wstring m_name;		// 파티명
	int m_max_playercount;		// 파티 최대 인원 수
	int m_cur_playercount;		// 파티 현재 인원 수

	PlayerInfoPtr m_owner;		// 파티장
	int m_owner_index;			// 아래 vector 에서 파티장의 index

	std::vector<PlayerInfoPtr> m_player_vec;	// 파티원 관리 벡터, nullptr 인 item 이 있을 수 있음
public:
	uint32_t GetID() const { return m_party_id; }
	std::wstring GetName() const { return m_name; }
	int GetMaxCapacity() const { return m_max_playercount; }
	int GetPlayerCount() const { return m_cur_playercount; }
private:
	// m_player_vec 에서 빈 공간을 좌측부터 검색하여 빈 index를 return
	int GetEmptySpace()
	{
		for (int i = 0; i < m_max_playercount; i++)
		{
			if (m_player_vec[i] == nullptr)
				return i;
		}
		return m_max_playercount;
	}

	// 해당 플레이어를 현재 파티에서 제거함
	void RemovePlayer(PlayerInfoPtr inpPlayer)
	{
		for (auto& item : m_player_vec)
		{
			if (item == inpPlayer)
			{
				item = nullptr;
				return;
			}
		}
	}

	void SetOwner(int index)
	{
		m_owner = m_player_vec[index];
		m_owner_index = index;
	}
protected:
	PlayerPartyInfo() : m_party_id(0U), m_name(), m_max_playercount(0), m_cur_playercount(0),
		m_owner(nullptr), m_owner_index(0), m_player_vec() {}
	void Initialize(uint32_t inPartyID, PlayerInfoPtr creater, std::wstring inParty_name, int inMax_capacity);
public:
	static PlayerPartyInfoPtr Create(uint32_t inPartyID, PlayerInfoPtr creater, std::wstring inParty_name, int inMax_capacity) {
		PlayerPartyInfo* newInfo = new PlayerPartyInfo();
		PlayerPartyInfoPtr retPtr;
		retPtr.reset(newInfo);
		retPtr->Initialize(inPartyID, creater, inParty_name, inMax_capacity);
		return retPtr;
	}
public:
	// 현재 파티원이 없는 경우
	bool IsEmpty() const
	{
		if (m_cur_playercount <= 0)
			return true;
		return false;
	}
	// 현재 파티인원이 다 찬 경우 true
	bool IsFull() const
	{
		if (m_max_playercount == m_cur_playercount)
			return true;
		return false;
	}
	// 해당 플레이어가 방장인경우 true
	bool IsOwner(PlayerInfoPtr inpPlayer) const
	{
		if (m_owner == inpPlayer)
			return true;
		return false;
	}

	// player 를 파티에 추가
	// 호출 전 full 체크 필수
	// 참가된 위치(index)를 return
	int Participate(PlayerInfoPtr requester)
	{
		++m_cur_playercount;
		auto index = GetEmptySpace();
		m_player_vec[index] = requester;
		return index;
	}

	// player를 파티에서 제거
	// 방장이 바뀐경우 새로운 방장의 index (0~3)
	// 방장이 바뀌지 않았다면 -1
	int Exit(PlayerInfoPtr requester)
	{
		// player를 현재 파티에서 제거
		RemovePlayer(requester);

		// 퇴장 인원이 방장인지 확인
		if (IsOwner(requester))
		{	// 방장인 경우
			// 새로운 방장을 검색 , 남아있는 인원 중 가장 좌측의 player를 새로운 방장으로
			for (int i = 0; i < m_max_playercount; i++)
			{	// 새로운 방장!
				if (requester == m_player_vec[i])
				{
					SetOwner(i);
					return i;
				}
			}
		}
		// 방장이 변경되지 않음
		return -1;
	}

	// 파티장이 특정 파티원을 강퇴시킴
	// 0~3 index
	void Kick(int index)
	{
		m_player_vec[index] = nullptr;
	}

	// 파티장의 권한을 타인에게 양도
	// 양도된 파티원의 Player Info를 return
	PlayerInfoPtr TransferPartyOwner(int index)
	{
		if (m_player_vec[index] != nullptr)
		{
			m_owner = m_player_vec[index];
			return m_owner;
		}
	}

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
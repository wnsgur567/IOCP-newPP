#pragma once
class PlayerPartyInfo;
using PlayerPartyInfoPtr = std::shared_ptr<PlayerPartyInfo>;
class PlayerPartyInfo : public ISerializable
{
	friend class PlayerPartyManager;

	// �ϳ��� ��Ƽ�� ���� ������ �ִ� �ο� ��
	constexpr static int MAX_CONSTRAINT = 4;

	uint32_t m_party_id;		// ��Ƽ ���� number
	std::wstring m_name;		// ��Ƽ��
	int m_max_playercount;		// ��Ƽ �ִ� �ο� ��
	int m_cur_playercount;		// ��Ƽ ���� �ο� ��

	PlayerInfoPtr m_owner;		// ��Ƽ��
	int m_owner_index;			// �Ʒ� vector ���� ��Ƽ���� index

	std::vector<PlayerInfoPtr> m_player_vec;	// ��Ƽ�� ���� ����, nullptr �� item �� ���� �� ����
public:
	uint32_t GetID() const { return m_party_id; }
	std::wstring GetName() const { return m_name; }
	int GetMaxCapacity() const { return m_max_playercount; }
	int GetPlayerCount() const { return m_cur_playercount; }
private:
	// m_player_vec ���� �� ������ �������� �˻��Ͽ� �� index�� return
	int GetEmptySpace()
	{
		for (int i = 0; i < m_max_playercount; i++)
		{
			if (m_player_vec[i] == nullptr)
				return i;
		}
		return m_max_playercount;
	}

	// �ش� �÷��̾ ���� ��Ƽ���� ������
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
	// �ش� ��ġ�� player�� �������� ����
	void SetOwner(PlayerInfoPtr inpPlayer, int index)
	{
		m_owner = inpPlayer;
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
	// ���� ��Ƽ���� ���� ���
	bool IsEmpty() const
	{
		if (m_cur_playercount <= 0)
			return true;
		return false;
	}
	// ���� ��Ƽ�ο��� �� �� ��� true
	bool IsFull() const
	{
		if (m_max_playercount == m_cur_playercount)
			return true;
		return false;
	}
	// �ش� �÷��̾ �����ΰ�� true
	bool IsOwner(PlayerInfoPtr inpPlayer) const
	{
		if (m_owner == inpPlayer)
			return true;
		return false;
	}

	// player �� ��Ƽ�� �߰�
	// ȣ�� �� full üũ �ʼ�
	// ������ ��ġ(index)�� return
	int Participate(PlayerInfoPtr requester)
	{
		++m_cur_playercount;
		auto index = GetEmptySpace();
		m_player_vec[index] = requester;
		return index;
	}

	// player�� ��Ƽ���� ����
	// ������ �ٲ��� ���ο� ������ playerinfo
	// ������ �ٲ��� �ʾҴٸ� nullptr
	PlayerInfoPtr Exit(PlayerInfoPtr requester)
	{
		// player�� ���� ��Ƽ���� ����
		RemovePlayer(requester);
		--m_cur_playercount;

		// ���� �ο��� �������� Ȯ��
		if (IsOwner(requester))
		{	// ������ ���
			// ���ο� ������ �˻� , �����ִ� �ο� �� ���� ������ player�� ���ο� ��������
			for (int i = 0; i < m_max_playercount; i++)
			{	// ���ο� ����!
				if (requester == m_player_vec[i])
				{
					SetOwner(m_player_vec[i], i);
					return m_player_vec[i];
				}
			}
		}
		// ������ ������� ����
		return nullptr;
	}

	// ��Ƽ���� Ư�� ��Ƽ���� �����Ŵ
	bool Kick(PlayerInfoPtr inpinfo)
	{
		for (auto& item : m_player_vec) {
			if (item != nullptr && item == inpinfo)
			{
				--m_cur_playercount;
				item = nullptr;
			}
		}
	}

	// ��Ƽ���� ������ Ÿ�ο��� �絵	
	void TransferPartyOwner(PlayerInfoPtr inpinfo)
	{
		for (int i = 0; i < m_max_playercount; i++)
		{
			if (m_player_vec[i] != nullptr && m_player_vec[i] == inpinfo)
				SetOwner(inpinfo, i);
		}
	}

	// ISerializable��(��) ���� ��ӵ�
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
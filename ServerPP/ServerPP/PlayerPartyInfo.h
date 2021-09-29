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
	int m_owner_index;
	std::vector<PlayerInfoPtr> m_player_vec;	// ��Ƽ�� ���� ����
public:
	uint32_t GetID() const { return m_party_id; }
	std::wstring GetName() const { return m_name; }
	int GetMaxCapacity() const { return m_max_playercount; }
	int GetPlayerCount() const { return m_cur_playercount; }
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
	// ��Ƽ ��û ���μ���
	void RequestToParticipate(PlayerInfoPtr inRequester)
	{
		// ��û�� ����� ĳ���� ������ ��Ƽ�忡�� ����		
		auto charInfo = inRequester->GetCharacterInfo();

	}
	// ��Ƽ�� �����ϴ� ���μ���
	void Participate(PlayerInfoPtr requester)
	{
		// ��Ƽ ������ ������ ������ ���
		// ��û�ڸ� ��Ƽ�� ���Խ�Ű�� 
		// ���� ��Ƽ�����Դ� requester�� ������
		// requester ���Դ� ���� ��Ƽ���� ������ ����
	}
	// ��Ƽ���� ������ ���μ���
	void Exit(PlayerInfoPtr requester)
	{
		// ��Ƽ���� �����ϴ� ���
		// �����̶�� ������ �絵�ϴ� ��� �߰�
		// ��Ƽ���鿡�� ���� ������ ����

		// ��Ƽ �ο����� 0���� �Ǹ� �ش� ��Ƽ�� �ı�
	}
	// ��Ƽ���� ��Ƽ���� �����Ű�� ���μ���
	void Kick(PlayerInfoPtr p)
	{
		// ������ Ư�� ��Ƽ���� �����Ŵ
	}
	// ��Ƽ���� ������ Ÿ�ο��� �絵�ϴ� ���μ���
	void TransferPartyOwner(PlayerInfoPtr p)
	{

	}

	// ISerializable��(��) ���� ��ӵ�
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
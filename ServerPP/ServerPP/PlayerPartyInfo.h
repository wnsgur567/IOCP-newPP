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
	int m_owner_index;
	std::vector<PlayerInfoPtr> m_player_vec;	// 파티원 관리 벡터
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
	// 파티 신청 프로세스
	void RequestToParticipate(PlayerInfoPtr inRequester)
	{
		// 요청한 사람의 캐릭터 정보를 파티장에게 전송		
		auto charInfo = inRequester->GetCharacterInfo();

	}
	// 파티로 참여하는 프로세스
	void Participate(PlayerInfoPtr requester)
	{
		// 파티 가입을 방장이 수락한 경우
		// 신청자를 파티에 가입시키고 
		// 기존 파티원에게는 requester의 정보를
		// requester 에게는 기존 파티원의 정보를 전송
	}
	// 파티에서 나가는 프로세스
	void Exit(PlayerInfoPtr requester)
	{
		// 파티에서 퇴장하는 경우
		// 방장이라면 권한을 양도하는 기능 추가
		// 파티원들에게 퇴장 정보를 전송

		// 파티 인원수가 0명이 되면 해당 파티를 파괴
	}
	// 파티장이 파티원을 강퇴시키는 프로세스
	void Kick(PlayerInfoPtr p)
	{
		// 방장이 특정 파티원을 강퇴시킴
	}
	// 파티장의 권한을 타인에게 양도하는 프로세스
	void TransferPartyOwner(PlayerInfoPtr p)
	{

	}

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
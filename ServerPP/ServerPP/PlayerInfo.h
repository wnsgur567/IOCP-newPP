#pragma once

class Sector;
using SectorPtr = std::shared_ptr<Sector>;

// Player
// 내부에 character info 있음
// 추가적으로 위치정보 같은것들
class PlayerInfo : public NetGameObject
{
	friend class NetGameObjectManager;
private:
	std::weak_ptr<IOCP_Base::IOCPSessionBase> m_pOwnerSession;
	CharacterInfoPtr m_character_info;
	Vector3 m_posisition;
	SectorPtr m_current_sector;		// 현재 플레이어가 속한 sector ptr
	PlayerInfo();

public:
	void SetSession(IOCP_Base::IOCPSessionBasePtr inOwnerSession) { m_pOwnerSession = inOwnerSession; }
	IOCP_Base::IOCPSessionBasePtr GetSession() { return m_pOwnerSession.lock(); }
	void SetCharacterInfo(CharacterInfoPtr inInfo) { m_character_info = inInfo; }
	CharacterInfoPtr GetCharacterInfo() { return m_character_info; }
	void SetPosition(float x, float y, float z) { m_posisition.x = x; m_posisition.y = y; m_posisition.z = z; }
	Vector3 GetPosition() { return m_posisition; }
	void SetSector(SectorPtr inpSector) { m_current_sector = inpSector; }
	SectorPtr GetSector() { return m_current_sector; }
public:
	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;

	// NetGameObject을(를) 통해 상속됨
	virtual void OnCreated() override;
	virtual void BeforeDestroy() override;

	// player의 Network ID 값이 일치하고
	// player가 현재 선택한 character의 ID 값이 일치하면
	// true
	bool operator==(const PlayerInfo& other) {
		if (other.GetNetID() == this->GetNetID() &&
			other.m_character_info->character_id == this->m_character_info->character_id)
			return true;		
		return false;
	}
	bool operator!=(const PlayerInfo& other) {
		return !(this->operator==(other));
	}
};
using PlayerInfoPtr = std::shared_ptr<PlayerInfo>;


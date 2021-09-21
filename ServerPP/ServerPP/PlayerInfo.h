#pragma once

// Player
// 내부에 character info 있음
// 추가적으로 위치정보 같은것들
class PlayerInfo : public NetGameObject , public ISerializable
{
private:
	CharacterInfoPtr m_character_info;
	Vector3 m_posisition;
public:
	PlayerInfo();
	void SetCharacterInfo(CharacterInfoPtr inInfo) { m_character_info = inInfo; }
	Vector3 GetPosition() { return m_posisition; }

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using PlayerInfoPtr = std::shared_ptr<PlayerInfo>;


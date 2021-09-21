#pragma once

// Player
// ���ο� character info ����
// �߰������� ��ġ���� �����͵�
class PlayerInfo : public NetGameObject , public ISerializable
{
private:
	CharacterInfoPtr m_character_info;
	Vector3 m_posisition;
public:
	PlayerInfo();
	void SetCharacterInfo(CharacterInfoPtr inInfo) { m_character_info = inInfo; }
	Vector3 GetPosition() { return m_posisition; }

	// ISerializable��(��) ���� ��ӵ�
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using PlayerInfoPtr = std::shared_ptr<PlayerInfo>;


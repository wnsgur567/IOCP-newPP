#pragma once

class PlayerInfo : public NetGameObject , public ISerializable
{
private:
	CharacterInfoPtr m_character_info;
	Vector3 m_posisition;
public:

	// ISerializable��(��) ���� ��ӵ�
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using PlayerInfoPtr = std::shared_ptr<PlayerInfo>;


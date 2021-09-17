#pragma once

class PlayerInfo : public NetGameObject , public ISerializable
{
private:
	CharacterInfoPtr m_character_info;
	Vector3 m_posisition;
public:

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using PlayerInfoPtr = std::shared_ptr<PlayerInfo>;


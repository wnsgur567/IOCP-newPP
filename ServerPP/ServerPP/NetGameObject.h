#pragma once


class NetGameObject : public ISerializable
{
	friend class NetGameObjectManager;
public:
	enum class ENetGameObjectType
	{
		None,

		Player = 1
	};
private:
	uint64_t m_net_id;		// 게임 세계에 첫 생성 시 부여되는 고유 id
	ENetGameObjectType m_type;
	bool m_isPlayable;		// user 가 직접 조작하는 object 인지
public:
	NetGameObject(ENetGameObjectType type) : m_net_id(), m_type(type), m_isPlayable(false) {}
	virtual bool IsEqual(const NetGameObject& other);
	uint64_t GetNetID() const;

	virtual void OnCreated() = 0;
	virtual void BeforeDestroy() = 0;

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};

using NetGameObjectPtr = std::shared_ptr<NetGameObject>;


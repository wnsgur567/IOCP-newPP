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
	uint64_t m_net_id;		// ���� ���迡 ù ���� �� �ο��Ǵ� ���� id
	ENetGameObjectType m_type;
	bool m_isPlayable;		// user �� ���� �����ϴ� object ����
public:
	NetGameObject(ENetGameObjectType type) : m_net_id(), m_type(type), m_isPlayable(false) {}
	virtual bool IsEqual(const NetGameObject& other);
	uint64_t GetNetID() const;

	virtual void OnCreated() = 0;
	virtual void BeforeDestroy() = 0;

	// ISerializable��(��) ���� ��ӵ�
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};

using NetGameObjectPtr = std::shared_ptr<NetGameObject>;


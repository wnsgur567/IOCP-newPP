#pragma once


class NetGameObject
{
private:
	uint64_t m_net_id;		// ���� ���迡 ù ���� �� �ο��Ǵ� ���� id
	bool m_isPlayable;		// user �� ���� �����ϴ� object ����
public:
	virtual bool IsEqual(const NetGameObject& other);	
};


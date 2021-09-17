#pragma once


class NetGameObject
{
private:
	uint64_t m_net_id;		// 게임 세계에 첫 생성 시 부여되는 고유 id
	bool m_isPlayable;		// user 가 직접 조작하는 object 인지
public:
	virtual bool IsEqual(const NetGameObject& other);	
};


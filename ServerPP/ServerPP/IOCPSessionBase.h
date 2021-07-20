#pragma once


class IOCPSessionBase : public SessionBase
{
protected:
	std::queue<SendPacketPtr> m_sendQueue;
public:
	IOCPSessionBase();
};
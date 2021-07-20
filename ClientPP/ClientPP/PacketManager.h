#pragma once

class PacketManager : public Singleton<PacketManager>
{
	friend class Singleton;
	friend class IOCPNetworkManager;
public:
	using psize_t = unsigned __int32;
	using id_t = unsigned __int32;

private:
	PacketManager()
		: m_capacity(STREAMPOOLCAPACITY),
		m_recvpacket_pool(), m_sendpacket_pool()
	{}
public:
	~PacketManager();

	bool Initialize(LPVOID) override;
	void Finalize() override;
private:
	psize_t m_capacity;

	// Ǯ���� ��Ŷ�� ������� �ʵ��� hold �� ���� �����̳���...
	std::list<AcceptPacketPtr> m_acceptpacket_container;
	std::list<RecvPacketPtr> m_recvpacket_container;
	std::list<SendPacketPtr> m_sendpacket_container;

	// iocp�� ���� ��Ʈ�� Ǯ��
	// �Ϲ� ����ȭ�� ��� ���ϴ� �Ѱ����� ��Ī ��ų ��
	std::queue<AcceptPacketPtr> m_acceptpacket_pool;
	std::queue<RecvPacketPtr> m_recvpacket_pool;
	std::queue<SendPacketPtr> m_sendpacket_pool;

public:
	AcceptPacketPtr GetAcceptPacketFromPool();
	void RetrieveAcceptPacket(AcceptPacketPtr inpPacket);
	RecvPacketPtr GetRecvPacketFromPool();
	void RetrieveRecvPacket(RecvPacketPtr inpPacket);
	SendPacketPtr GetSendPacketFromPool();
	void RetrieveSendPacket(SendPacketPtr inpPacket);
};

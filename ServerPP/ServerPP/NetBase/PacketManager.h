#pragma once

namespace NetBase
{
	// Initialize �� ���ο� ������ Args ����ü�� �����Ͽ� �Ѱ��� ��
	// send packet ������ stream �� ���۸� alloc ���� ����
	// pooling �� stream �� ������ �۾� ���� ����� sendpacket �� packing �Լ��� �Ѱ��ٰ�
	class PacketManager : public MyBase::Singleton<PacketManager>
	{
	private:
		friend class Singleton;
		friend class IOCPNetworkManager;
	public:
		using packetSize_t = PacketBase::packetSize_t;
		using packetId_t = PacketBase::packetId_t;
	private:
		PacketManager()
			: m_recvpacket_pool(), m_sendpacket_pool()
		{}
	public:
		~PacketManager();

		struct InitializeArgs
		{
			::gsize64_t numberOfAcptPacket;		// Pooling �� AcceptPackt ����
			::gsize64_t numberOfRecvPacket;		// Pooling �� RecvPacket ����
			packetSize_t capacityOfRecvBuffer;	// RecvPacket �� buffer �ִ� �뷮
			::gsize64_t numberOfSendPacket;		// Pooling �� SendPacket ����
			packetSize_t capacityOfSendBuffer;	// SendPacket �� buffer �ִ� �뷮
		};

		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;
	private:

		// Ǯ���� ��Ŷ�� ������� �ʵ��� hold �� ���� �����̳���...
		std::list<AcceptPacketPtr> m_acceptpacket_container;
		std::list<RecvPacketPtr> m_recvpacket_container;
		std::list<SendPacketPtr> m_sendpacket_container;
		std::list<OutputMemoryStreamPtr> m_sendStream_container;

		// iocp�� ���� ��Ʈ�� Ǯ��
		// �Ϲ� ����ȭ�� ��� ���ϴ� �Ѱ����� ��Ī ��ų ��
		std::queue<AcceptPacketPtr> m_acceptpacket_pool;
		std::queue<RecvPacketPtr> m_recvpacket_pool;
		std::queue<SendPacketPtr> m_sendpacket_pool;
		std::queue<OutputMemoryStreamPtr> m_sendStream_queue;

	public:
		AcceptPacketPtr GetAcceptPacketFromPool();
		void RetrieveAcceptPacket(AcceptPacketPtr inpPacket);
		RecvPacketPtr GetRecvPacketFromPool();
		void RetrieveRecvPacket(RecvPacketPtr inpPacket);
		SendPacketPtr GetSendPacketFromPool();
		void RetrieveSendPacket(SendPacketPtr inpPacket);
		OutputMemoryStreamPtr GetSendStreamFromPool();	// send packet ȸ�� �� ���� ó��
	};

}
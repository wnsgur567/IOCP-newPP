#pragma once

// Packet ���� 
// 1. ��Ŷ�� �� ���� (size)
// 2. ��Ŷ�� �ѹ� (id)
// 3. Stream (Protocol + Data)

// ù GetReady ���� head �� size �� id ũ�⸸ŭ �̵���Ŵ

// for recv buffer
class IOCPInputMemoryStream : public InputMemoryStream
{
private:
	friend class RecvPacket;
	friend class PacketManager;
public:
	using bufsize_t = ::gsize_t;
	using packetId_t = ::gid_t;
protected:
	void GetReady();
	packetId_t GetID();
public:
	IOCPInputMemoryStream(const bufsize_t);
};
using IOCPInputMemoryStreamPtr = std::shared_ptr<IOCPInputMemoryStream>;

// for send buffer
class IOCPOutputMemoryStream : public OutputMemoryStream
{
private:
	friend class SendPacket;
	friend class PacketManager;
public:
	using bufsize_t = ::gsize_t;
	using packetId_t = ::gid_t;
protected:
	void GetReady();
	void SetSize(const bufsize_t);
	void SetID(const packetId_t);
public:
	IOCPOutputMemoryStream(const bufsize_t);
};
using IOCPOutputMemoryStreamPtr = std::shared_ptr<IOCPOutputMemoryStream>;
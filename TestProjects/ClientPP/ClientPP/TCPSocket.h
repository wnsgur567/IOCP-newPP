#pragma once

class TCPSocket
{
private:
	SOCKET	m_socket;
	bool	m_isBlock;
public:
	TCPSocket(SOCKET inSocket);
	~TCPSocket();

	SOCKET			GetSock() const;
	bool			IsBlockSock() const;

	bool			Bind(const SocketAddress& inAddress);
	bool			Listen(int inClients);
	TCPSocketPtr	Accept(SocketAddress& outAddress);
	bool			Connect(const SocketAddress& inServerAddr);
	bool			SetNonBlockingMode(bool inIsNonBlock);
public:
	int				Send(const void* inData, int inLen);
	int				Recv(void* outData, int inLen);
	bool			operator==(const TCPSocket& inOther);
};
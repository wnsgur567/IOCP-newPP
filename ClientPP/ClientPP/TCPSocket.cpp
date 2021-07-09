#include "base.h"

TCPSocket::TCPSocket(SOCKET inSocket)
	: m_socket(inSocket), m_isBlock(true)
{

}

TCPSocket::~TCPSocket()
{
	printf("~TCPSocket");
	closesocket(m_socket);
}

SOCKET TCPSocket::GetSock() const
{
	return m_socket;
}

bool TCPSocket::IsBlockSock() const
{
	return m_isBlock;
}

bool TCPSocket::Bind(const SocketAddress& inAddress)
{
	int retval = bind(m_socket, (sockaddr*)&inAddress, sizeof(inAddress));
	if (retval == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Bind");
		return false;
	}
	return true;
}
bool TCPSocket::Listen(int inClients)
{
	int retval = listen(m_socket, inClients);
	if (retval == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Listen");
		return false;
	}
	return true;
}
TCPSocketPtr TCPSocket::Accept(SocketAddress& outFromAddress)
{
	socklen_t len = sizeof(sockaddr_in);
	SOCKET _new_sock = accept(m_socket, (sockaddr*)&(outFromAddress.GetSockAddr()), &len);

	if (EWOULDBLOCK == _new_sock)
	{
		SocketUtil::ReportError("TCPSocket::Accept Nonblock");
		return nullptr;
	}

	if (_new_sock == INVALID_SOCKET)
	{
		SocketUtil::ReportError("TCPSocket::Accept");
		return nullptr;
	}

	return std::make_shared<TCPSocket>(_new_sock);
}

bool TCPSocket::Connect(const SocketAddress& inServerAddr)
{
	int retval = connect(m_socket, (const sockaddr*)&inServerAddr.GetSockAddr(), inServerAddr.GetAddrSize());
	if (retval == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Connect");
		return false;
	}
	return true;
}

bool TCPSocket::SetNonBlockingMode(bool inIsNonBlock)
{
	u_long arg = inIsNonBlock ? 1 : 0;
	int retval = ioctlsocket(m_socket, FIONBIO, &arg);
	if (retval == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::SetNonBlockingMode");
		return false;
	}

	m_isBlock = !inIsNonBlock;
	return true;
}

int	TCPSocket::Send(const void* inData, int inLen)
{
	int sendbyte = send(m_socket, (const char*)inData, inLen, 0);
	if (sendbyte == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Send");
		// 양수부는 정상 종료임
		return -SocketUtil::GetLastError();
	}
	else if (sendbyte == SOCKET_END)
	{
		return SOCKET_END;
	}
	return sendbyte;
}

int	TCPSocket::Recv(void* outData, int inLen)
{
	int recvbyte = recv(m_socket, (char*)outData, inLen, 0);
	if (recvbyte == SOCKET_ERROR)
	{
		SocketUtil::ReportError("TCPSocket::Recv");
		return -SocketUtil::GetLastError();
	}
	else if (recvbyte == SOCKET_END)
	{
		return SOCKET_END;
	}
	return recvbyte;
}

bool TCPSocket::operator==(const TCPSocket& inOther)
{
	if (inOther.m_socket != m_socket)
		return false;
	return true;
}

#include "base.h"

std::unique_ptr<NetworkManagerClient> Singleton<NetworkManagerClient>::sInstance;

bool NetworkManagerClient::Init(const char* inIP, u_short inPort)
{
	//家南 积己
	m_sock = SocketUtil::CreateTCPSocket();
	if (m_sock == nullptr)
	{
		return false;
	}

	sockaddr_in _addr;
	int retval = inet_pton(AF_INET, inIP, &_addr.sin_addr);
	if (retval == SOCKET_ERROR)
		return false;

	// set address
	SocketAddress serverAddr(_addr.sin_addr.s_addr, inPort);
	m_serveraddr = serverAddr;

	// connect
	if (false == m_sock->Connect(m_serveraddr))
		return false;

	// debug
	m_serveraddr.Print("Connet to Server [IP] : %s / [Port] : %d");

	return true;
}

NetworkManagerClient::~NetworkManagerClient()
{
	SocketUtil::CleanUp();
}

bool NetworkManagerClient::Initialize()
{
	// wsa init
	if (false == SocketUtil::Init())
		return false;

	if (false == NetworkManagerClient::Init(SERVERIP, htons(SERVERPORT)))
		return false;

	return true;
}

void NetworkManagerClient::Finalize()
{
}

bool NetworkManagerClient::DoFrame()
{
	return true;
}

TCPSocketPtr NetworkManagerClient::GetSockPtr() const
{
	return m_sock;
}

bool NetworkManagerClient::Recv(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket)
{
	// 烙矫 力累
	// 悼扁
	int size = inpSock->Recv(&outRecvPacket->m_target_recvbytes, sizeof(RecvPacket::psize_t));
	if (size == SOCKET_ERROR)
		return false;

	size = inpSock->Recv(outRecvPacket->m_buf, outRecvPacket->m_target_recvbytes);
	if (size == SOCKET_ERROR)
		return false;


	return true;
}

bool NetworkManagerClient::Send(const TCPSocketPtr inpSock, SendPacketPtr inpPacket)
{
	int retval = inpSock->Send(inpPacket->m_buf, inpPacket->m_target_sendbytes);
	if (retval == SOCKET_ERROR)
		return false;

	return true;
}

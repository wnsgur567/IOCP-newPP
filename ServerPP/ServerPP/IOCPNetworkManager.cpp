#include "base.h"

Implementation_sInstance(IOCPNetworkManager);

bool IOCPNetworkManager::OnAccepted(TCPSocketPtr, SocketAddress)
{
	return false;
}

bool IOCPNetworkManager::OnDisconnected()
{
	return false;
}

bool IOCPNetworkManager::OnCompleteRecv(RecvPacketPtr)
{
	return false;
}

bool IOCPNetworkManager::OnCompleteSend(SendPacketPtr)
{
	return false;
}

#include "base.h"


IOCPSessionBase::IOCPSessionBase()
{
	m_pRecvPacket = PacketManager::sInstance->GetRecvPacketFromPool();
}

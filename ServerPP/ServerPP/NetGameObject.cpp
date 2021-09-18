#include "IOCPNet_RootHeader.h"

bool NetGameObject::IsEqual(const NetGameObject& other)
{
    if (m_net_id == other.m_net_id)
        return true;
    return false;
}

uint64_t NetGameObject::GetNetID() const
{
    return m_net_id;
}

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

int NetGameObject::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
{
    int size = 0;
    WriteToStream(out_stream, m_net_id);
    WriteToStream(out_stream, (int32_t)m_type);
    return size;
}

int NetGameObject::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
    int size = 0;
    ReadFromStream(in_stream, m_net_id);
    int32_t type_raw;
    ReadFromStream(in_stream, type_raw);
    m_type = (ENetGameObjectType)type_raw;
    return size;
}

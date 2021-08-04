#include "base.h"

void IOCPInputMemoryStream::GetReady()
{
	//m_head = sizeof(bufsize_t) + sizeof(packetId_t);
}

IOCPInputMemoryStream::packetId_t IOCPInputMemoryStream::GetID()
{
	packetId_t id;
	memcpy(&id, m_buffer, sizeof(id));
	return id;
}

IOCPInputMemoryStream::IOCPInputMemoryStream(const bufsize_t inCapacity)
	: InputMemoryStream(inCapacity)
{

}

void IOCPInputMemoryStream::Decryption()
{
	//CipherManager::sInstance->Decryption()
}

void IOCPOutputMemoryStream::GetReady()
{
	m_head = sizeof(bufsize_t) + sizeof(packetId_t);
}

void IOCPOutputMemoryStream::SetSize(const bufsize_t inSize)
{
	BYTE* ptr = m_buffer;
	memcpy(ptr, &inSize, sizeof(inSize));
}

void IOCPOutputMemoryStream::SetID(const packetId_t inID)
{
	BYTE* ptr = m_buffer + sizeof(bufsize_t);
	memcpy(ptr, &inID, sizeof(inID));
}

IOCPOutputMemoryStream::IOCPOutputMemoryStream(const bufsize_t inCapacity)
	: OutputMemoryStream(inCapacity)
{
}

void IOCPOutputMemoryStream::Encryption()
{
}


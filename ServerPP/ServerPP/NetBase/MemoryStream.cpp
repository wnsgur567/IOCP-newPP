#include "MemoryStream.h"
#include <algorithm>

namespace NetBase
{
#pragma region Recv (InputStream)

	InputMemoryStream::InputMemoryStream(size_t inByteCount)
		:m_buffer(nullptr),
		m_isBufferOwner(true),
		m_head(0),
		m_length(0),
		m_capacity(inByteCount)
	{
		m_buffer = new BYTE[m_capacity];
		memset(m_buffer, 0, m_capacity);	
	}

	// 외부 버퍼 사용
	// 소유권 x
	InputMemoryStream::InputMemoryStream(BYTE* inBuffer, size_t inByteCount, bool inIsOwner)
		: m_buffer(inBuffer),
		m_isBufferOwner(inIsOwner),
		m_head(0),
		m_length(0),
		m_capacity(inByteCount)
	{

	}

	// 버퍼를 새로 만들어 복사함
	// 소유권 0
	InputMemoryStream::InputMemoryStream(const InputMemoryStream& inOther)
		: m_buffer(nullptr),
		m_isBufferOwner(true),
		m_head(inOther.m_head),
		m_length(inOther.m_length),
		m_capacity(inOther.m_capacity)
	{
		m_buffer = new BYTE[m_capacity];
		memcpy(m_buffer, inOther.m_buffer, m_capacity);
	}

	InputMemoryStream& InputMemoryStream::operator=(const InputMemoryStream& inOther)
	{
		if (m_isBufferOwner)
			delete[] m_buffer;
		m_capacity = inOther.m_capacity;
		m_head = inOther.m_head;
		m_length = inOther.m_length;
		m_isBufferOwner = true;
		m_buffer = new BYTE[m_capacity];
		memcpy(m_buffer, inOther.m_buffer, m_capacity);

		return *this;
	}

	// 소유권 있는 경우에만 해제
	InputMemoryStream::~InputMemoryStream()
	{
		if (m_isBufferOwner)
			delete[] m_buffer;
	}

	void InputMemoryStream::SetLenth(const size_t inLength)
	{
		m_length = inLength;
	}

	BYTE* InputMemoryStream::GetBufferPtr()
	{
		return m_buffer;
	}

	size_t InputMemoryStream::GetLength() const
	{
		return m_length;
	}

	size_t InputMemoryStream::GetCapacity() const
	{
		return m_capacity;
	}

	void InputMemoryStream::Clear()
	{
		m_head = 0;
		m_length = 0;
	}

	size_t InputMemoryStream::GetRemainDataSize() const
	{
		return (m_capacity - m_length);
	}

	// endian 반영되지 않는 메모리 읽기
	// endian 반영되는것은 netbase 의 ReadFromBinStream 을 이용할 것
	void InputMemoryStream::Read(void* outData, size_t inByteCount)
	{
		memcpy(outData, m_buffer + m_head, inByteCount);
		m_head = m_head + inByteCount;
	}

#pragma endregion


#pragma region Send (OutputStream)
	void OutputMemoryStream::ReAllocBuffer(size_t inNewLength)
	{
		if (inNewLength < m_capacity)
			return;

		BYTE* tmpBuffer = new BYTE[inNewLength];
		for (size_t i = 0; i < m_capacity; i++)
		{
			tmpBuffer[i] = m_buffer[i];
		}
		delete[] m_buffer;
		m_buffer = tmpBuffer;
		m_capacity = inNewLength;
	}

	OutputMemoryStream::OutputMemoryStream(size_t inCapacity)
		:m_buffer(nullptr), m_head(0), m_capacity(inCapacity)
	{
		m_buffer = new BYTE[m_capacity];
	}

	OutputMemoryStream::OutputMemoryStream(const OutputMemoryStream& inOther)
		: m_buffer(nullptr),
		m_head(inOther.m_head),
		m_capacity(inOther.m_capacity)
	{
		m_buffer = new BYTE[m_capacity];
		memcpy(m_buffer, inOther.m_buffer, inOther.m_head);
	}

	OutputMemoryStream& OutputMemoryStream::operator=(const OutputMemoryStream& inOther)
	{
		delete[] m_buffer;
		m_head = inOther.m_head;
		m_capacity = inOther.m_capacity;
		m_buffer = new BYTE[m_capacity];
		memcpy(m_buffer, inOther.m_buffer, inOther.m_head);

		return *this;
	}

	OutputMemoryStream::~OutputMemoryStream()
	{
		delete[] m_buffer;
	}

	BYTE* OutputMemoryStream::GetBufferPtr()
	{
		return m_buffer;
	}

	size_t OutputMemoryStream::GetLength() const
	{
		return m_head;
	}

	size_t OutputMemoryStream::GetCapacity() const
	{
		return m_capacity;
	}

	void OutputMemoryStream::Clear()
	{
		m_head = 0;
	}

	void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
	{
		size_t result_head = m_head + inByteCount;

		if (result_head > m_capacity)
			ReAllocBuffer(std::max(m_capacity * 2, result_head));

		std::memcpy(m_buffer + m_head, inData, inByteCount);

		m_head = result_head;
	}

	void OutputMemoryStream::Write(const std::string& inString)
	{
		int length = static_cast<int>(inString.length());
		Write<int>(length);
		memcpy(m_buffer + m_head, inString.c_str(), length);
		m_head = m_head + sizeof(int) + length;
	}
#pragma endregion

}
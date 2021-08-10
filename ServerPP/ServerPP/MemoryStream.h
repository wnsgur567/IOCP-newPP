#pragma once

using BYTE = unsigned char;
#include<memory>
#include<string>

class InputMemoryStream;
using InputMemoryStreamPtr = std::shared_ptr<InputMemoryStream>;

// recv buffer ��
class InputMemoryStream
{
protected:
	BYTE* m_buffer;				// ����
	bool  m_isBufferOwner;		// ������ �������� ���� ��쿡�� �����ϵ���								

	size_t m_head;				// �б� ������ ���� head �� ��ġ
	size_t m_length;			// ���ۿ� �� �뷮
	size_t m_capacity;			// �ִ� �뷮
public:
	InputMemoryStream(size_t inByteCount);
	InputMemoryStream(BYTE* inBuffer, size_t inByteCount, bool inIsOwner = false);
	InputMemoryStream(const InputMemoryStream& inOther);
	InputMemoryStream& operator=(const InputMemoryStream& inOther);
	~InputMemoryStream();
public:
	void SetLenth(const size_t inLength);
	BYTE* GetBufferPtr();
	size_t GetLength() const;
	size_t GetCapacity() const;
	void Clear();
public:
	size_t GetRemainDataSize() const;
	void Read(void* outData, size_t inByteCount);
	template<typename T> void Read(T& outData);
};

template<typename T>
inline void InputMemoryStream::Read(T& outData)
{
	static_assert(
		std::is_arithmetic<T>::value ||
		std::is_enum<T>::value,
		"Generi Write only supports primitive data type"
		);

	Read(&outData, sizeof(T));
}


class OutputMemoryStream;
using OutputMemoryStreamPtr = std::shared_ptr<OutputMemoryStream>;

// send buffer ��
class OutputMemoryStream
{
protected:
	BYTE* m_buffer;				// ����
	size_t m_head;				// ���� ������ ���� head�� ��ġ
	size_t m_capacity;			// �ִ� �뷮

	void ReAllocBuffer(size_t inNewLength);
public:
	OutputMemoryStream(size_t inCapacity);
	OutputMemoryStream(const OutputMemoryStream& inOther);
	OutputMemoryStream& operator=(const OutputMemoryStream& inOther);
	~OutputMemoryStream();
public:
	BYTE* GetBufferPtr();
	size_t GetLength() const;
	size_t GetCapacity() const;
	void Clear();
public:
	void Write(const void* inData, size_t inByteCount);
	template<typename T> void Write(const T& inData);
	void Write(const std::string& inString);
};

// only primitive type
template<typename T>
inline void OutputMemoryStream::Write(const T& inData)
{
	static_assert(
		std::is_arithmetic<T>::value ||
		std::is_enum<T>::value,
		"Generi Write only supports primitive data type"
		);

	Write(&inData, sizeof(inData));
}

#pragma once

#include <memory>
#include <ostream>
#include <istream>
#include <streambuf>

// stream buf doc1 : https://c-annotationskr.sourceforge.io/cplusplus24.html
// stream buf doc2 : http://korea.gnu.org/manual/release/iostream/iostream_5.html

// sync()
// 
//
// p �� put
// pbase : put ������ ���� ��ġ
// epptr : put ������ ��
// setp : pbase �� epptr �� ����
// pptr : ���� write buffer �� head ��ġ ������
// pbump : put ptr ���� n ��ŭ �̵�// 
// underflow() : ���۰� ���� �� ȣ���
// 
// g �� get
// eback : get ������ ����
// egptr : get ������ ��
// setg : eback �� egptr �� ����
// gptr : ���� readbuffer �� head ��ġ ������
// gbump : put ptr ���� n ��ŭ �̵�
// overflow() : ���۰� �� �� ȣ���
//
// 
//  
//


// trait �� Ư�� ���� ����ü
// ������ Ÿ�ӿ� �̳��� � ������ �˾Ƴ��� ����

using BYTE = unsigned char;


#define BUFSIZE 64	// test ��
class InputStreamBuffer;
using InputStreamBufferPtr = std::shared_ptr<InputStreamBuffer>;
class InputStreamBuffer : public std::streambuf
{
private:
	int m_head;
	int m_capacity;
	BYTE* m_buffer;

private:
	InputStreamBuffer()
		: m_head(0), m_capacity(0), m_buffer(nullptr) {}
public:
	~InputStreamBuffer()
	{
		if (m_buffer != nullptr)
			delete[] m_buffer;
	}

	static InputStreamBufferPtr Create(int capacity)
	{
		InputStreamBufferPtr retPtr;
		retPtr.reset(new InputStreamBuffer());

		retPtr->m_head = 0;
		retPtr->m_capacity = capacity;
		retPtr->m_buffer = new BYTE[retPtr->m_capacity];

		retPtr->Initialize();

		return retPtr;
	}


	void Initialize()
	{
		// for write buffer
		// stream buffer ���� m_buffer �� ���� ũ�⸦ �˷���
		m_head = 0;
		setp(m_buffer, m_buffer + m_capacity);
	}


};

class OutputStreamBuffer;
using OutputStreamBufferPtr = std::shared_ptr< OutputStreamBuffer>;
class OutputStreamBuffer : public std::streambuf
{
private:
	int m_head;
	int m_capacity;
	char* m_buffer;

private:
	OutputStreamBuffer()
		: m_head(0), m_capacity(0), m_buffer(nullptr) {}
public:
	~OutputStreamBuffer()
	{
		if (m_buffer != nullptr)
			delete[] m_buffer;
	}

	static OutputStreamBufferPtr Create(int capacity)
	{
		OutputStreamBufferPtr retPtr;
		retPtr.reset(new OutputStreamBuffer());

		retPtr->m_head = 0;
		retPtr->m_capacity = capacity;
		retPtr->m_buffer = new char[retPtr->m_capacity];

		retPtr->Initialize();

		return retPtr;
	}

	void Initialize()
	{
		m_head = 0;
		setp(m_buffer, m_buffer + m_capacity);
	}

	int sync() override
	{
		if (pptr() > pbase())
		{			
			write(m_head, m_buffer, pptr() - pbase());
			setp(m_buffer, m_buffer + m_capacity);
		}
		return 0;
	}
	int overflow(int c)
	{
		printf("stream::overflow()");
		sync();
		if (c != EOF)
		{
			*pptr() = c;
			pbump(1);
		}
		return c;
	}

private:
	//void write(int head, )
};
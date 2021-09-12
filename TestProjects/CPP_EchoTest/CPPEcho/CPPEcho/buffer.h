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
// p 는 put
// pbase : put 영역의 시작 위치
// epptr : put 영역의 끝
// setp : pbase 와 epptr 을 셋팅
// pptr : 현제 write buffer 의 head 위치 포인터
// pbump : put ptr 에서 n 만큼 이동// 
// underflow() : 버퍼가 쓰일 때 호출됨
// 
// g 는 get
// eback : get 영역의 시작
// egptr : get 영역의 끝
// setg : eback 와 egptr 을 셋팅
// gptr : 현재 readbuffer 의 head 위치 포인터
// gbump : put ptr 에서 n 만큼 이동
// overflow() : 버퍼가 찰 때 호출됨
//
// 
//  
//


// trait 란 특성 정보 구조체
// 컴파일 타임에 이놈이 어떤 놈인지 알아내기 위한

using BYTE = unsigned char;


#define BUFSIZE 64	// test 용
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
		// stream buffer 에게 m_buffer 의 실제 크기를 알려줌
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
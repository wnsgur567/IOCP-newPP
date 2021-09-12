#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <type_traits>

// 참고 : https://bab2min.tistory.com/613

// --------------------- std::basic_string<T> -------------------------//
// c++ string 은 char , wchar 등 여러개의 타입에 대응될 필요가 있는데 이를 위해 basic_string 템플릿을 사용하여 정의함
// using std::string = std::basic_string<char>
// using std::string = std::basic_string<wchar_t> 등으로 정의된다 생각하면 됨

// --------------------- std::enable_if -------------------------//
// std::is_fundamental<_Ty>::value -> bool 값 (fundamental 인 경우에만 true)
// std::enable_if< bool , _Ty >::type -> bool 값이 true 이면 _Ty type 으로 정의됨
//      ex) std::enable_if<true, int>::type val 은 int val 과 동일 (_Ty는 기본 void)
//          타입이 정의되지 않는 경우 컴파일 되지 않음

// size 는 32만 사용하겠음
// 어처피 64 사용할 크기는 못보냄

namespace Utils
{
	using byte = unsigned char;

	// is_fundamental이 true일때만 이 함수가 정의됩니다.
	template<class _Ty, typename _StreamElem = char>
	inline typename std::enable_if<std::is_fundamental<_Ty>::value&& std::_Is_character<_StreamElem>::value>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const _Ty& v)
	{
		if (!os.write((const _StreamElem*)&v, sizeof(_Ty)))
			// write 실패한 경우엔 exception을 던져주는걸로
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(_Ty).name() + "' failed");
	}


	template<class _Ty, typename _StreamElem = char>
	inline typename std::enable_if<std::is_fundamental<_Ty>::value&& std::_Is_character<_StreamElem>::value>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, _Ty& v)
	{
		if (!is.read((_StreamElem*)&v, sizeof(_Ty)))
			// 읽기 실패시 throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(_Ty).name() + "' failed");
	}

#pragma region Write (Serialization)
	// 바이너리 스트림으로 _Ty 타입을 직렬화하는 함수입니다
	template <class _Ty, typename _StreamElem = char>
	inline void WriteToBinStream(std::basic_ostream<_StreamElem>& os, const _Ty& v)
	{
		WriteToBinStreamImpl<_Ty, _StreamElem>(os, v);
	}

	// 바이너리 스트림으로부터 _Ty 타입을 역직렬화하는 함수입니다
	template<class _Ty, typename _StreamElem = char>
	inline void ReadFromBinStream(std::basic_istream<_StreamElem>& is, _Ty& v)
	{
		ReadFromBinStreamImpl<_Ty, _StreamElem>(is, v);
	}

	// 위 함수와 같은데 결과 값을 리턴해주는 형태
	template <class _Ty, typename _StreamElem = char>
	inline _Ty ReadFromBinStream(std::basic_istream<_StreamElem>& is)
	{
		_Ty v;
		ReadFromBinStreamImpl<_Ty, _StreamElem>(is, v);
		return v;
	}

	//////////////// container 

	// for string
	template<class _Ty, typename _StreamElem = char>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::basic_string<_Ty>& v)
	{
		// 크기를 저장합니다
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// string 을 저장
		for (auto& e : v)
		{
			WriteToBinStream<_Ty, _StreamElem>(os, e);
		}
	}

	// for vector
	template<class _Ty, typename _StreamElem = char>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::vector<_Ty>& v)
	{
		// 먼저 vector의 크기를 저장합니다
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// 그리고 각 요소를 저장
		for (auto& e : v)
		{
			WriteToBinStream<_Ty, _StreamElem>(os, e);
		}
	}

	// for list
	template<class _Ty, typename _StreamElem = char>
	inline void WriteToBinStreamImpl(std::ostream& os, const typename std::list<_Ty>& l)
	{
		// list의 크기를 저장
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(l.size()));
		// 각 요소를 저장
		for (auto& item : l)
		{
			WriteToBinStream<_Ty, _StreamElem>(os, item);
		}
	}

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem = char>
	inline void WriteToBinStreamImpl(std::ostream& os, const typename std::pair<_Ty1, _Ty2>& v)
	{
		WriteToBinStream<_Ty1, _StreamElem>(os, v.first);
		WriteToBinStream<_Ty2, _StreamElem>(os, v.second);
	}

	// for map
	template<class _Ty1, class _Ty2, typename _StreamElem = char>
	inline void WriteToBinStreamImpl(std::ostream& os, const typename std::map<_Ty1, _Ty2>& v)
	{
		// map의 크기를 저장하시고
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// 그리고 각 요소를 저장
		for (auto& p : v)
		{
			WriteToBinStream<_Ty1, _Ty2, _StreamElem>(os, p);
		}
	}

	// for set
	template<class _Ty, typename _StreamElem = char>
	inline void WriteToBinStreamImpl(std::ostream& os, const typename std::set<_Ty>& v)
	{
		// 크기를 저장합니다
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// 그리고 각 요소를 저장
		for (auto& e : v)
		{
			WriteToBinStream<_Ty, _StreamElem>(os, e);
		}
	}


#pragma endregion

#pragma region Read (DeSerialization)



	//////////////// container 

	// for string
	template<class _Ty, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::basic_string<_Ty>& v)
	{
		v.resize(ReadFromBinStream<uint32_t, _StreamElem>(is));
		for (auto& e : v)
		{
			ReadFromBinStream<_Ty, _StreamElem>(is, e);
		}
	}

	// for vector
	template<class _Ty, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::vector<_Ty>& v)
	{
		v.resize(ReadFromBinStream<uint32_t, _StreamElem>(is));
		for (auto& e : v)
		{
			ReadFromBinStream <_Ty, _StreamElem>(is, e);
		}
	}

	// for list
	template<class _Ty, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::list<_Ty>& l)
	{
		l.resize(ReadFromBinStream<uint32_t, _StreamElem>(is));
		for (auto& item : l)
		{
			ReadFromBinStream<_Ty, _StreamElem>(is, item);
		}
	}

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::pair<_Ty1, _Ty2>& v)
	{
		v.first = ReadFromBinStream<_Ty1, _StreamElem>(is);
		v.second = ReadFromBinStream<_Ty2, _StreamElem>(is);
	}

	// for map
	template<class _Ty1, class _Ty2, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::map<_Ty1, _Ty2>& v)
	{
		size_t len = ReadFromBinStream<uint32_t, _StreamElem>(is);
		v.clear();
		for (size_t i = 0; i < len; ++i)
		{
			v.emplace(ReadFromBinStream<std::pair<_Ty1, _Ty2>, _StreamElem>(is));
		}
	}

	// for set
	template<class _Ty, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::set<_Ty>& v)
	{
		size_t len = ReadFromBinStream<uint32_t, _StreamElem>(is);
		v.clear();
		for (size_t i = 0; i < len; ++i)
		{
			v.emplace(ReadFromBinStream<_Ty, _StreamElem>(is));
		}
	}

#pragma endregion

}
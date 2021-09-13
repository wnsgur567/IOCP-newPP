#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include "ByteUtils.h"

// ���� : https://bab2min.tistory.com/613

// --------------------- std::basic_string<T> -------------------------//
// c++ string �� char , wchar �� �������� Ÿ�Կ� ������ �ʿ䰡 �ִµ� �̸� ���� basic_string ���ø��� ����Ͽ� ������
// using std::string = std::basic_string<char>
// using std::string = std::basic_string<wchar_t> ������ ���ǵȴ� �����ϸ� ��

// --------------------- std::enable_if -------------------------//
// std::is_fundamental<_Ty>::value -> bool �� (fundamental �� ��쿡�� true)
// std::enable_if< bool , _Ty >::type -> bool ���� true �̸� _Ty type ���� ���ǵ�
//      ex) std::enable_if<true, int>::type val �� int val �� ���� (_Ty�� �⺻ void)
//          Ÿ���� ���ǵ��� �ʴ� ��� ������ ���� ����

// size �� 32�� ����ϰ���
// ��ó�� 64 ����� ũ��� ������

namespace Utils
{
	using byte = unsigned char;

#pragma region forward Declaration
	// for string
	template<class _Ty, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::basic_string<_Ty>& v);
	template<class _Ty, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::basic_string<_Ty>& v);

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::pair<_Ty1, _Ty2>& v);
	template<class _Ty1, class _Ty2, typename _StreamElem = char>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::pair<_Ty1, _Ty2>& v);
#pragma endregion	

	// character type �� ���� �����ڵ尡 ��� serialize �Ǵ��� Ȯ���ϰ�  �� ������
	// unicode �� big endian �����̶� �Ƹ� ���ϴ°� ���� �� ������...

	// integral type
	template<class _Ty, typename _StreamElem>
	inline typename std::enable_if<std::is_integral<_Ty>::value&& std::_Is_character<_StreamElem>::value>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const _Ty& v)
	{		
		byte outBytes[sizeof(_Ty)];
		Utils::BitConverter::GetBytes(v, outBytes);

		if (!os.write((const _StreamElem*)outBytes, sizeof(_Ty)))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(_Ty).name() + "' failed");
	}	
	
	// float 
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const float_t& v)
	{
		byte outBytes[sizeof(float_t)];
		Utils::BitConverter::GetBytes(v, outBytes);

		if (!os.write((const _StreamElem*)outBytes, sizeof(float_t)))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(float_t).name() + "' failed");
	}
	// double
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const double_t& v)
	{
		byte outBytes[sizeof(double_t)];
		Utils::BitConverter::GetBytes(v, outBytes);

		if (!os.write((const _StreamElem*)outBytes, sizeof(double_t)))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(double_t).name() + "' failed");
	}

	// integral type
	template<class _Ty, typename _StreamElem>
	inline typename std::enable_if<std::is_integral<_Ty>::value&& std::_Is_character<_StreamElem>::value>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, _Ty& v)
	{
		
		byte inBytes[sizeof(_Ty)];
		if (!is.read((_StreamElem*)inBytes, sizeof(_Ty)))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(_Ty).name() + "' failed");
		v = Utils::BitConverter::BytesToVal<_Ty>(inBytes);		
	}

	// float
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, float_t& v)
	{
		byte inBytes[sizeof(float_t)];
		if (!is.read((_StreamElem*)inBytes, sizeof(float_t)))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(float_t).name() + "' failed");
		v = Utils::BitConverter::BytesToFloat(inBytes);
	}

	// double
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, double_t& v)
	{
		byte inBytes[sizeof(double_t)];
		if (!is.read((_StreamElem*)inBytes, sizeof(double_t)))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(double_t).name() + "' failed");
		v = Utils::BitConverter::BytesToDouble(inBytes);
	}

#pragma region Write (Serialization)
	// ���̳ʸ� ��Ʈ������ _Ty Ÿ���� ����ȭ�ϴ� �Լ��Դϴ�
	template <class _Ty, typename _StreamElem>
	inline void WriteToBinStream(std::basic_ostream<_StreamElem>& os, const _Ty& v)
	{
		WriteToBinStreamImpl(os, v);
	}

	// ���̳ʸ� ��Ʈ�����κ��� _Ty Ÿ���� ������ȭ�ϴ� �Լ��Դϴ�
	template<class _Ty, typename _StreamElem>
	inline void ReadFromBinStream(std::basic_istream<_StreamElem>& is, _Ty& v)
	{
		ReadFromBinStreamImpl(is, v);
	}

	// �� �Լ��� ������ ��� ���� �������ִ� ����
	template <class _Ty, typename _StreamElem>
	inline _Ty ReadFromBinStream(std::basic_istream<_StreamElem>& is)
	{
		_Ty v;
		ReadFromBinStreamImpl(is, v);
		return v;
	}

	//////////////// container 

	// for string
	template<class _Ty, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::basic_string<_Ty>& v)
	{
		// ũ�⸦ �����մϴ�
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// string �� ����
		for (auto& e : v)
		{
			WriteToBinStream(os, e);
		}
	}

	// for vector
	template<class _Ty, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::vector<_Ty>& v)
	{
		// ���� vector�� ũ�⸦ �����մϴ�
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// �׸��� �� ��Ҹ� ����
		for (auto& e : v)
		{
			WriteToBinStream(os, e);
		}
	}

	// for list
	template<class _Ty, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::list<_Ty>& l)
	{
		// list�� ũ�⸦ ����
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(l.size()));
		// �� ��Ҹ� ����
		for (auto& item : l)
		{
			WriteToBinStream(os, item);
		}
	}

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::pair<_Ty1, _Ty2>& v)
	{
		WriteToBinStream(os, v.first);
		WriteToBinStream(os, v.second);
	}

	// for map
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::map<_Ty1, _Ty2>& v)
	{
		// map�� ũ�⸦ �����Ͻð�
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// �׸��� �� ��Ҹ� ����
		for (auto& p : v)
		{
			WriteToBinStream(os, p);
		}
	}

	// for set
	template<class _Ty, typename _StreamElem>
	inline void WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::set<_Ty>& v)
	{
		// ũ�⸦ �����մϴ�
		WriteToBinStream<uint32_t, _StreamElem>(os, static_cast<uint32_t>(v.size()));
		// �׸��� �� ��Ҹ� ����
		for (auto& e : v)
		{
			WriteToBinStream(os, e);
		}
	}


#pragma endregion

#pragma region Read (DeSerialization)



	//////////////// container 

	// for string
	template<class _Ty, typename _StreamElem>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::basic_string<_Ty>& v)
	{
		v.resize(ReadFromBinStream<uint32_t, _StreamElem>(is));
		for (auto& e : v)
		{
			ReadFromBinStream(is, e);
		}
	}

	// for vector
	template<class _Ty, typename _StreamElem>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::vector<_Ty>& v)
	{
		v.resize(ReadFromBinStream<uint32_t, _StreamElem>(is));
		for (auto& e : v)
		{
			ReadFromBinStream(is, e);
		}
	}

	// for list
	template<class _Ty, typename _StreamElem>
	inline void ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::list<_Ty>& l)
	{
		l.resize(ReadFromBinStream<uint32_t, _StreamElem>(is));
		for (auto& item : l)
		{
			ReadFromBinStream(is, item);
		}
	}

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem>
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
	template<class _Ty, typename _StreamElem>
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

	void StringTest();
	void VectorTest();
	void ListTest();
	void SetTest();
	void MapTest();
}
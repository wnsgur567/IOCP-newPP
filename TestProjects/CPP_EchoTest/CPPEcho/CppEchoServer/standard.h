#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <sstream>
#include "ByteUtils.h"
#include "ISerializable.h"
#include <cassert>

// ���� : https://bab2min.tistory.com/613

// character Ÿ���� byte �����Ǵ� int �� ȣ���
// ex ) wchar_t -> int16

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
	using byte_string = std::basic_string<byte>;

#pragma region forward Declaration
	template <class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStream(std::basic_ostream<_StreamElem>& os, const _Ty& v);
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStream(std::basic_istream<_StreamElem>& is, _Ty& v);

	// for string
	template<class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::basic_string<_Ty>& v);
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::basic_string<_Ty>& v);

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::pair<_Ty1, _Ty2>& v);
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::pair<_Ty1, _Ty2>& v);

#pragma endregion	

	// character type �� ���� �����ڵ尡 ��� serialize �Ǵ��� Ȯ���ϰ�  �� ������
	// unicode �� big endian �����̶� �Ƹ� ���ϴ°� ���� �� ������...
#pragma region Write (Serialization)
	// integral type
	template<class _Ty, typename _StreamElem>
	inline typename std::enable_if<std::is_integral<_Ty>::value&& std::_Is_character<_StreamElem>::value, int32_t>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const _Ty& v)
	{
		constexpr size_t write_size = sizeof(_Ty);

		byte outBytes[write_size];
		Utils::BitConverter::GetBytes(v, outBytes);

		if (!os.write((const _StreamElem*)outBytes, write_size))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(_Ty).name() + "' failed");

		return static_cast<int32_t>(write_size);
	}

	// float 
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value, int32_t>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const float_t& v)
	{
		constexpr size_t write_size = sizeof(float_t);

		byte outBytes[sizeof(float_t)];
		Utils::BitConverter::GetBytes(v, outBytes);

		if (!os.write((const _StreamElem*)outBytes, write_size))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(float_t).name() + "' failed");

		return static_cast<int32_t>(write_size);
	}
	// double
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value, int32_t>::type WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const double_t& v)
	{
		constexpr size_t write_size = sizeof(double_t);

		byte outBytes[write_size];
		Utils::BitConverter::GetBytes(v, outBytes);

		if (!os.write((const _StreamElem*)outBytes, write_size))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "writing type '" } + typeid(double_t).name() + "' failed");

		return static_cast<int32_t>(write_size);
	}




	//////////////// container 

	// for string
	template<class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::basic_string<_Ty>& v)
	{
		int32_t write_size = 0;

		// ũ�⸦ �����մϴ�
		write_size += WriteToBinStream<int32_t, _StreamElem>(os, static_cast<int32_t>(v.size()));
		// string �� ����
		for (auto& e : v)
		{
			write_size += WriteToBinStream(os, e);
		}

		return write_size;
	}

	// for vector
	template<class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::vector<_Ty>& v)
	{
		int32_t write_size = 0;

		// ���� vector�� ũ�⸦ �����մϴ�
		write_size += WriteToBinStream<int32_t, _StreamElem>(os, static_cast<int32_t>(v.size()));
		// �׸��� �� ��Ҹ� ����
		for (auto& e : v)
		{
			write_size += WriteToBinStream(os, e);
		}

		return write_size;
	}

	// for list
	template<class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::list<_Ty>& l)
	{
		int32_t write_size = 0;

		// list�� ũ�⸦ ����
		write_size += WriteToBinStream<int32_t, _StreamElem>(os, static_cast<int32_t>(l.size()));
		// �� ��Ҹ� ����
		for (auto& item : l)
		{
			write_size += WriteToBinStream(os, item);
		}

		return write_size;
	}

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::pair<_Ty1, _Ty2>& v)
	{
		int32_t write_size = 0;

		write_size += WriteToBinStream(os, v.first);
		write_size += WriteToBinStream(os, v.second);

		return write_size;
	}

	// for map
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::map<_Ty1, _Ty2>& v)
	{
		int32_t write_size = 0;

		// map�� ũ�⸦ �����Ͻð�
		write_size += WriteToBinStream<int32_t, _StreamElem>(os, static_cast<int32_t>(v.size()));
		// �׸��� �� ��Ҹ� ����
		for (auto& p : v)
		{
			write_size += WriteToBinStream(os, p);
		}

		return write_size;
	}

	// for set
	template<class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(std::basic_ostream<_StreamElem>& os, const typename std::set<_Ty>& v)
	{
		int32_t write_size = 0;
		// ũ�⸦ �����մϴ�
		write_size += WriteToBinStream<int32_t, _StreamElem>(os, static_cast<int32_t>(v.size()));
		// �׸��� �� ��Ҹ� ����
		for (auto& e : v)
		{
			write_size += WriteToBinStream(os, e);
		}
		return write_size;
	}


#pragma endregion

#pragma region Read (DeSerialization)

	// integral type
	template<class _Ty, typename _StreamElem>
	inline typename std::enable_if<std::is_integral<_Ty>::value&& std::_Is_character<_StreamElem>::value, int32_t>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, _Ty& v)
	{
		constexpr size_t read_size = sizeof(_Ty);

		byte inBytes[read_size];
		if (!is.read((_StreamElem*)inBytes, read_size))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(_Ty).name() + "' failed");
		v = Utils::BitConverter::BytesToVal<_Ty>(inBytes);

		return static_cast<int32_t>(read_size);
	}

	// float
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value, int32_t>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, float_t& v)
	{
		constexpr size_t read_size = sizeof(float_t);

		byte inBytes[read_size];
		if (!is.read((_StreamElem*)inBytes, read_size))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(float_t).name() + "' failed");
		v = Utils::BitConverter::BytesToFloat(inBytes);

		return static_cast<int32_t>(read_size);
	}

	// double
	template<typename _StreamElem>
	inline typename std::enable_if<std::_Is_character<_StreamElem>::value, int32_t>::type ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, double_t& v)
	{
		constexpr size_t read_size = sizeof(double_t);

		byte inBytes[read_size];
		if (!is.read((_StreamElem*)inBytes, read_size))
			// �б� ���н� throw
			throw std::ios_base::failure(std::string{ "reading type '" } + typeid(double_t).name() + "' failed");
		v = Utils::BitConverter::BytesToDouble(inBytes);

		return static_cast<int32_t>(read_size);
	}

	//////////////// container 

	// for string
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::basic_string<_Ty>& v)
	{
		int32_t read_size = 0;
		int32_t string_size = 0;
		ReadFromBinStream<int32_t, _StreamElem>(is, string_size);
		read_size += sizeof(int32_t);
		v.resize(string_size);
		for (auto& e : v)
		{
			read_size += ReadFromBinStream(is, e);
		}
		return read_size;
	}

	// for vector
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::vector<_Ty>& v)
	{
		int32_t read_size = 0;
		int32_t vector_size;
		ReadFromBinStream<int32_t, _StreamElem>(is, vector_size);
		read_size += sizeof(int32_t);
		v.resize(vector_size);
		for (auto& e : v)
		{
			read_size += ReadFromBinStream(is, e);
		}
		return read_size;
	}

	// for list
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::list<_Ty>& l)
	{
		int32_t read_size = 0;
		int32_t list_size;
		ReadFromBinStream<int32_t, _StreamElem>(is, list_size);
		read_size += sizeof(int32_t);
		l.resize(list_size);
		for (auto& item : l)
		{
			read_size += ReadFromBinStream(is, item);
		}
		return read_size;
	}

	// for pair
	template<class _Ty1, class _Ty2, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::pair<_Ty1, _Ty2>& v)
	{
		int32_t first_read_size = ReadFromBinStream<_Ty1, _StreamElem>(is, v.first);
		int32_t second_read_size = ReadFromBinStream<_Ty2, _StreamElem>(is, v.second);
		return first_read_size + second_read_size;
	}

	// for map
	template<class _Ty1, class _Ty2, typename _StreamElem = char>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::map<_Ty1, _Ty2>& v)
	{
		int32_t read_size = 0;
		int32_t map_size;
		ReadFromBinStream<int32_t, _StreamElem>(is, map_size);
		read_size += sizeof(int32_t);
		v.clear();
		for (int32_t i = 0; i < map_size; ++i)
		{
			std::pair<_Ty1, _Ty2> item;
			read_size += ReadFromBinStream<std::pair<_Ty1, _Ty2>, _StreamElem>(is, item);
			v.insert(item);
		}
		return read_size;
	}

	// for set
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(std::basic_istream<_StreamElem>& is, typename std::set<_Ty>& v)
	{
		int32_t read_size = 0;
		int32_t set_size;
		ReadFromBinStream<int32_t, _StreamElem>(is, set_size);
		read_size += sizeof(int32_t);
		v.clear();
		for (int32_t i = 0; i < set_size; ++i)
		{
			_Ty item;
			read_size += ReadFromBinStream<_Ty, _StreamElem>(is, item);
			v.insert(item);
		}
		return read_size;
	}

#pragma endregion

	// write for ISerializable Class Ptr 
	template<typename _StreamElem>
	inline int32_t WriteToBinStreamImpl(
		std::basic_ostream<_StreamElem>& os,
		ISerializable* class_ptr)
	{
		return class_ptr->Serialize(os);
	}

	// read for ISerializable Class Ptr
	template<typename _StreamElem>
	inline int32_t ReadFromBinStreamImpl(
		std::basic_istream<_StreamElem>& is,
		ISerializable* class_ptr)
	{	
		return class_ptr->DeSerialize(is);
	}	

	// ���̳ʸ� ��Ʈ������ _Ty Ÿ���� ����ȭ�ϴ� �Լ��Դϴ�
	template <class _Ty, typename _StreamElem>
	inline int32_t WriteToBinStream(std::basic_ostream<_StreamElem>& os, const _Ty& v)
	{
		return WriteToBinStreamImpl(os, v);
	}

	// ���̳ʸ� ��Ʈ�����κ��� _Ty Ÿ���� ������ȭ�ϴ� �Լ��Դϴ�
	template<class _Ty, typename _StreamElem>
	inline int32_t ReadFromBinStream(std::basic_istream<_StreamElem>& is, _Ty& v)
	{
		return ReadFromBinStreamImpl(is, v);
	}

	void SerializationIntegralTest();
	void SerializationFloatingTest();
	void characterTest();
	void StringTest();
	void VectorTest();
	void ListTest();
	void SetTest();
	void MapTest();
}
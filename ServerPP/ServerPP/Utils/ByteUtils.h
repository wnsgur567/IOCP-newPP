#pragma once

#include <iostream>
#include <type_traits>
#include <cstdint>
#include <vector>

#pragma warning (push)
#pragma warning (disable : 26451)

// 참고 : https://github.com/YanjieHe/BitConverter/blob/main/include/bit_converter/bit_converter.hpp

namespace Utils
{
	// net : big endian
	// output 값은 big 으로 통double_t일함
	// little endian 일 경우 교차시키기

	class BitConverter
	{

	public:
		using byte = unsigned char;
		static constexpr int BYTE_SIZE = sizeof(byte) * 8;
	private:
		static bool m_isLittleEndian;
		void EndianCheck()
		{

			int num = 1;
			if (*(char*)&num == 1)
			{
				m_isLittleEndian = true;
#ifdef __DEBUG
				printf("This machine is 'Littel Endian'\n");
#endif
			}
			else
			{
				m_isLittleEndian = false;
#ifdef __DEBUG
				printf("This machine is 'Big Endian'\n");
#endif
			}
			printf("Endiancheck");
		}
		BitConverter()
		{
			EndianCheck();
		}
	private:
		template <typename OutputIt>
		static OutputIt create_bytes_from_bits(const std::vector<bool>& bits,
			OutputIt output_it) {
			for (int i = 0; i < static_cast<int>(bits.size() / 8); i++) {
				uint8_t b = 0;
				for (int j = 0; j < 8; j++) {
					b = b + (bits[i * 8 + j] << j);
				}
				*output_it = b;
				output_it++;
			}
			return output_it;
		}
	public:
		static bool IsLittelEndian()
		{
			return m_isLittleEndian;
		}

	public:
		template<typename T>
		static typename std::enable_if
			<std::is_fundamental<T>::value>::type GetBytes(T inValue, byte outBytes[])
		{
			byte* ptr = outBytes;
			int type_size = static_cast<int>(sizeof(T));
			int bytes_length = type_size * 8;
			if (m_isLittleEndian)
			{
				for (int i = type_size - 1; i >= 0; i--)
				{
					*ptr = static_cast<byte>(
						((inValue >> (bytes_length - BYTE_SIZE - i * BYTE_SIZE)) & 0xFF)
						);
					++ptr;
				}
			}
			else
			{
				for (int i = 0; i < type_size; i++)
				{
					*ptr = static_cast<byte>(
						((inValue >> (bytes_length - BYTE_SIZE - i * BYTE_SIZE)) & 0xFF)
						);
					++ptr;
				}
			}
		}

		template<typename T>
		static typename std::enable_if<std::is_fundamental<T>::value, T>::type BytesToVal(byte inBytes[])
		{
			T result = T();
			int type_size = static_cast<int>(sizeof(T));
			//int bytes_length = static_cast<int>(sizeof(T)) * 8;

			byte* ptr = inBytes;
			if (m_isLittleEndian)
			{
				for (int i = 0; i < type_size; i++) {
					result = result + ((static_cast<T>(*ptr) << (BYTE_SIZE * i)));
					++ptr;
				}
			}
			else
			{
				for (int i = 0; i < type_size; i++) {
					result = result + ((static_cast<T>(*ptr) << (BYTE_SIZE * (type_size - 1 - i))));
					++ptr;
				}
			}
			return result;
		}

	public:
		// https://ko.wikipedia.org/wiki/%EB%B6%80%EB%8F%99%EC%86%8C%EC%88%98%EC%A0%90

		static void GetBytes(float_t inValue, byte outBytes[])
		{
			int32_t tmp;
			memcpy(&tmp, &inValue, sizeof(int32_t));
			return GetBytes(tmp, outBytes);
		}

		static float_t BytesToFloat(byte inBytes[])
		{
			int32_t tmp = BytesToVal<int32_t>(inBytes);
			float_t ret;
			memcpy(&ret, &tmp, sizeof(int32_t));
			return ret;
		}

		static void GetBytes(double_t inValue, byte outBytes[])
		{
			int64_t tmp;
			memcpy(&tmp, &inValue, sizeof(int32_t));
			return GetBytes(tmp, outBytes);
		}

		static double_t BytesToDouble(byte inBytes[])
		{
			int64_t tmp = BytesToVal<int64_t>(inBytes);
			double_t ret;
			memcpy(&ret, &tmp, sizeof(int64_t));
			return ret;
		}

	public:
		static void DebugPrint(byte bytes[], size_t size)
		{
			for (size_t i = 0; i < size; i++)
			{
				printf("%02X ", bytes[i]);
			}
			printf("\n");
		}

	private:
		friend class BitConverterStaticConstructor;
	};


	// for static initialize
	// static var is alloc in memory before main()
	class BitConverterStaticConstructor
	{
		static BitConverter bitconverter;
	};

	void IntegerTest();
	void UIntegerTest();
	void FloatTest();
}

#pragma warning (pop)
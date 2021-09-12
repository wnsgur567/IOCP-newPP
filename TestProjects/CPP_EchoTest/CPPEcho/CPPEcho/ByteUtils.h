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
	// output 값은 big 으로 통일함
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
		static typename std::enable_if<std::is_integral<T>::value>::type GetBytes(T inValue, byte outBytes[])
		{
			byte* ptr = outBytes;
			int type_size = static_cast<int>(sizeof(T));
			int bytes_length = type_size * 8;
			if (IsLittelEndian())
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
		static typename std::enable_if<std::is_integral<T>::value, T>::type BytesToVal(byte inBytes[])
		{
			T result = T();
			int type_size = static_cast<int>(sizeof(T));
			//int bytes_length = static_cast<int>(sizeof(T)) * 8;

			byte* ptr = inBytes;
			if (IsLittelEndian())
			{
				for (int i = 0; i < type_size; i++) {
					result = result + (static_cast<T>(*ptr) << (8 * i));
					++ptr;
				}
			}
			else
			{
				for (int i = 0; i < type_size; i++) {
					result = result + (static_cast<T>(*ptr) << (8 * (type_size - 1 - i)));
					++ptr;
				}
			}
			return result;
		}

	public:
		static void GetBytes(float_t inValue, byte outBytes[])
		{
			std::vector<bool> bits;
			bits.reserve(sizeof(float_t) * 8);	// 8 * 8

			// 1.sign bit
			bits.push_back(inValue < 0);

			// 2. exponent
			int exponent;
			float_t mantissa = std::frexpf(inValue, &exponent);
			exponent = (exponent - 1) + 127;	// bias 127

			for (int i = 0; i < 8; i++)
			{	// dec -> bin
				bits.push_back(exponent % 2);
				exponent = exponent / 2;
			}
			std::reverse(bits.begin() + 1, bits.begin() + 1 + 8);

			// 3. mantissa
			mantissa = mantissa * 2 - 1;
			for (int i = 0; i < 23; i++)
			{
				mantissa = mantissa * 2;
				if (mantissa >= 1.0f)
				{
					mantissa = mantissa - 1.0f;
					bits.push_back(true);
				}
				else
				{
					bits.push_back(false);
				}
			}


			byte* ptr = outBytes;
			if (m_isLittleEndian)
			{	// litte
				std::reverse(bits.begin(), bits.end());
				create_bytes_from_bits<byte*>(bits, ptr);
			}
			else
			{	// big
				create_bytes_from_bits<byte*>(bits, ptr);
			}
		}

		static void GetBytes(double_t inValue, byte outBytes[])
		{
			std::vector<bool> bits;
			bits.reserve(sizeof(double_t) * 8);

			// 1. sign
			bits.push_back(inValue < 0);

			// 2. exponent
			int exponent;
			double_t mantissa = std::frexp(inValue, &exponent);
			exponent = (exponent - 1) + 1023;

			for (int i = 0; i < 11; i++)
			{	// dec to bin
				bits.push_back(exponent % 2);
				exponent = exponent / 2;
			}
			std::reverse(bits.begin() + 1, bits.begin() + 1 + 11);

			// 3. mantissa
			mantissa = mantissa * 2 - 1;
			for (int i = 0; i < 52; i++)
			{
				mantissa = mantissa * 2;
				if (mantissa >= 1.0)
				{
					mantissa = mantissa - 1.0;
					bits.push_back(true);
				}
				else
				{
					bits.push_back(false);
				}
			}

			byte* ptr = outBytes;
			if (IsLittelEndian()) 
			{
				std::reverse(bits.begin(), bits.end());
				create_bytes_from_bits(bits, ptr);
			}
			else
			{
				create_bytes_from_bits(bits, ptr);
			}
		}


		static float_t BytesToFloat(byte inBytes[])
		{
			std::vector<bool> bits;
			byte* ptr = inBytes;
			bits.reserve(sizeof(float_t) * 8);
			for (int i = 0; i < static_cast<int>(sizeof(float_t)); i++)
			{
				uint8_t b = *ptr;
				++ptr;
				for (int j = 0; j < 8; j++)
				{
					bits.push_back(b % 2);
					b = b / 2;
				}
			}
			if (m_isLittleEndian)
			{
				std::reverse(bits.begin(), bits.end());
			}

			int sign = bits[0] ? (-1) : (+1);
			int exponent = 0;
			for (int i = 0; i < 8; i++)
			{
				exponent = exponent + bits[1 + i] * (1 << (8 - 1 - i));
			}
			exponent = exponent - 127;
			float_t mantissa = 1.0;
			float_t cur = 0.5;
			for (int i = 0; i < 23; i++)
			{
				mantissa = mantissa + bits[1 + 8 + i] * cur;
				cur = cur / 2;
			}
			return sign * std::ldexp(mantissa, exponent);
		}

		static double_t BytesToDouble(byte inBytes[])
		{
			std::vector<bool> bits;
			byte* ptr = inBytes;
			bits.reserve(sizeof(double_t) * 8);
			for (int i = 0; i < static_cast<int>(sizeof(double_t)); i++) {
				uint8_t b = *ptr;
				++ptr;
				for (int j = 0; j < 8; j++) 
				{
					bits.push_back(b % 2);
					b = b / 2;
				}
			}

			if (IsLittelEndian())
			{
				std::reverse(bits.begin(), bits.end());
			}

			int sign = bits[0] ? (-1) : (+1);
			int exponent = 0;
			for (int i = 0; i < 11; i++) 
			{
				exponent = exponent + bits[1 + i] * (1 << (11 - 1 - i));
			}
			exponent = exponent - 1023;
			double_t mantissa = 1.0;
			double_t cur = 0.5;
			for (int i = 0; i < 52; i++)
			{
				mantissa = mantissa + bits[1 + 11 + i] * cur;
				cur = cur / 2;
			}
			return sign * std::ldexp(mantissa, exponent);
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
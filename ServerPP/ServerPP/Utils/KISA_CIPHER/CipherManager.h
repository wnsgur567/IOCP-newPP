#pragma once

#include <Windows.h>
#include "../Singleton.h"

namespace NetCipher
{
	typedef unsigned __int32	_DWORD;		// unsigned 4-byte data type

	class CipherManager : public MyBase::Singleton<CipherManager>
	{
	private:
		static constexpr size_t m_block_size = 16;	// 16 byte ( 16 * 8 = 128bit)
		_DWORD m_pdwRoundKey[32];
		// 16 byte (128 bit)
		BYTE m_pbUserKey[m_block_size] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 		// User secret key
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	public:
		CipherManager();

		bool Initialize(LPVOID) override;
		void Finalize() override;
	public:
		size_t Encryption(BYTE*, size_t);
		void Decryption(BYTE*, size_t);
	};
}
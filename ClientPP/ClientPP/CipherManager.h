#pragma once

#include <Windows.h>
#include "Singleton.h"

class CipherManager : public Singleton<CipherManager>
{
private:
	static constexpr size_t m_block_size = 16;	// 16 byte ( 16 * 8 = 128bit)
	DWORD m_pdwRoundKey[32];
	// 16 byte (128 bit)
	BYTE m_pbUserKey[m_block_size] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 		// User secret key
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

public:
	CipherManager();

	bool Initialize(LPVOID) override;
	void Finalize() override;
public:
	void Encryption(BYTE*, size_t);
	void Decryption(BYTE*, size_t);
};
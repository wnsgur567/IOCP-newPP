#include "KISA_CIPHER/KISA_SEED_ECB.h"
#include "CipherManager.h"

Implementation_sInstance(CipherManager);

CipherManager::CipherManager()
	: m_pdwRoundKey()
{
}

bool CipherManager::Initialize(LPVOID)
{
	// Derive roundkeys from user secret key
	SEED_KeySchedKey(m_pdwRoundKey, m_pbUserKey);
	return true;
}

void CipherManager::Finalize()
{

}

size_t CipherManager::Encryption(BYTE* inpData, size_t inSize)
{
	BYTE* ptr = inpData;
	size_t complete_size = 0;

	// add padding bits for encryption


	printf("\n\nEncrpyion...\n");

	while (complete_size < inSize)
	{
		SEED_Encrypt(ptr, m_pdwRoundKey);

		// debug
		for (int i = 0; i < m_block_size; i++)
			printf("%02X ", ptr[i]);
		printf("\n");

		ptr += m_block_size;
		complete_size += m_block_size;
	}

	return complete_size;
}

void CipherManager::Decryption(BYTE* inpData, size_t inSize)
{
	BYTE* ptr = inpData;
	size_t complete_size = 0;

	printf("\n\nDecrpyion...\n");

	while (complete_size < inSize)
	{
		SEED_Decrypt(ptr, m_pdwRoundKey);

		// debug
		for (int i = 0; i < m_block_size; i++)
			printf("%02X ", ptr[i]);
		printf("\n");

		ptr += m_block_size;
		complete_size += m_block_size;
	}
}
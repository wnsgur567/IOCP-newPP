#include "KISA_CIPHER/KISA_SEED_ECB.h"
#include "base.h"

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

void CipherManager::Encryption(BYTE* inpData, size_t inSize)
{
	BYTE* ptr = inpData;
	size_t complete_size = 0;
	
	while (complete_size < inSize)
	{
		//SEED_Encrypt(ptr, m_pdwRoundKey);

		ptr += m_block_size;
		complete_size += m_block_size;		
	}
}

void CipherManager::Decryption(BYTE* inpData, size_t inSize)
{
	BYTE* ptr = inpData;
	size_t complete_size = 0;

	while (complete_size < inSize)
	{
		//SEED_Decrypt(ptr, m_pdwRoundKey);

		ptr += m_block_size;
		complete_size += m_block_size;
	}
}

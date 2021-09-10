#include "KISA_SEED_ECB.h"
#include "CipherManager.h"

Implementation_sInstance(NetCipher::CipherManager);

namespace NetCipher
{
	CipherManager::CipherManager()
		: m_pdwRoundKey()
	{
	}

	bool CipherManager::Initialize(LPVOID) noexcept
	{
#ifdef  __DEBUG
		printf("----- CipherManager::Initialize() -----\n");
#endif //  __DEBUG

		// Derive roundkeys from user secret key
		SEED_KeySchedKey(m_pdwRoundKey, m_pbUserKey);

#ifdef  __DEBUG
		printf("----- CipherManager::Initialize() end -----\n");
#endif //  __DEBUG
		return true;
	}

	void CipherManager::Finalize() noexcept
	{
#ifdef  __DEBUG
		printf("----- CipherManager::Finalize() -----\n");
#endif //  __DEBUG
#ifdef  __DEBUG
		printf("----- CipherManager::Finalize() end -----\n");
#endif //  __DEBUG
	}

	size_t CipherManager::Encryption(BYTE* inpData, size_t inSize)
	{
		BYTE* ptr = inpData;
		size_t complete_size = 0;

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
			// debug
			printf("before:\n");
			for (int i = 0; i < m_block_size; i++)
				printf("%02X ", ptr[i]);
			printf("\n");

			SEED_Decrypt(ptr, m_pdwRoundKey);

			// debug
			printf("after:\n");
			for (int i = 0; i < m_block_size; i++)
				printf("%02X ", ptr[i]);
			printf("\n");

			ptr += m_block_size;
			complete_size += m_block_size;
		}
	}
}
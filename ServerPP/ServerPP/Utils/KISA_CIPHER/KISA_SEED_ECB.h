/*******************************************************************************
*
* FILE:         KISA_SEED_ECB.h
*
* DESCRIPTION:  header file for KISA_SEED_ECB.c
*
*******************************************************************************/


#pragma once

/******************************* Include files ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace NetCipher
{
	using BYTE = unsigned char;
	using _DWORD = unsigned __int32;	// unsigned 4-byte data type
	using _WORD = unsigned __int16;		// unsigned 2-byte data type

	/*************************** Function Declarations ****************************/

	void SEED_Encrypt(		/* encryption function */
		BYTE* pbData, 				// [in,out]	data to be encrypted
		_DWORD* pdwRoundKey			// [in]			round keys for encryption
	);
	void SEED_Decrypt(		/* decryption function */
		BYTE* pbData, 				// [in,out]	data to be decrypted
		_DWORD* pdwRoundKey			// [in]			round keys for decryption
	);
	void SEED_KeySchedKey(		/* key scheduling function */
		_DWORD* pdwRoundKey, 		// [out]		round keys for encryption or decryption
		BYTE* pbUserKey				// [in]			secret user key 
	);


	/*************************** END OF FILE **************************************/
}
#include "base.h"



int main()
{
	if (false == NetworkEngine::StaticInit())
		return 1;

	if (false == NetworkEngine::sInstance->Initialize(nullptr))
		return 1;

	if (false == NetworkEngine::sInstance->DoLoop())
		return 1;

	NetworkEngine::sInstance->Finalize();

	return 0;
}
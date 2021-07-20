#include "base.h"

int main()
{
	if (false == Engine::StaticInit())
		return 1;

	if (false == Engine::sInstance->Initialize(nullptr))
		return 1;

	if (false == Engine::sInstance->DoLoop())
		return 1;

	Engine::sInstance->Finalize();

	return 0;
}
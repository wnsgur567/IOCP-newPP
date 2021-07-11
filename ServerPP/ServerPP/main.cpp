#include "base.h"

int main()
{
	if (Engine::StaticInit())
	{
		if (false == Engine::sInstance->DoLoop())
			return 1;
	}

	Engine::sInstance->Finalize();
	return 0;
}
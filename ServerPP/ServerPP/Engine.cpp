#include "base.h"

Implementation_sInstance(Engine);

bool Engine::DoFrame()
{
	IOCPNetworkManager::sInstance->DoFrame();

	return true;
}

#include "base.h"

std::unique_ptr<Engine> Singleton<Engine>::sInstance;

bool Engine::Initialize()
{
	if (false == IOCPNetworkManager::StaticInit(SERVERPORT))
		return false;

	if (false == PacketManager::StaticInit())
		return false;

	if (false == IOCPSessionManager::StaticInit())
		return false;

	if (false == SignManager::StaticInit())
		return false;

	IOCPSessionManager::sInstance->RegistCreationFunction(IOCPSession::CreateSession);

	return true;
}

void Engine::Finalize()
{
}

bool Engine::DoFrame()
{
	return true;
}

bool Engine::DoLoop()
{
	bool quit = false;
	while (!quit)
	{
		if (false == DoFrame())
			quit = true;
	}

	return true;
}

#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Engine);

bool Engine::DoFrame()
{

	return true;
}

bool Engine::Initialize(LPVOID args)
{
	IOCP_Base::EngineBase<Engine, IOCPSession>::Initialize(args);

	return true;
}

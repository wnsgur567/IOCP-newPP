#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Engine);

bool Engine::DoFrame()
{

	return true;
}

bool Engine::Initialize(LPVOID args) noexcept
{
#ifdef  __DEBUG
	printf("\n----- Engine::Initialize() -----\n");
#endif //  __DEBUG

	// base initilize
	IOCP_Base::EngineBase<Engine, IOCPSession>::Initialize(args);

	// SQL 
	SQL::SQLManager::InitArgs sqlArgs;
	sqlArgs.ip = "49.1.218.29";
	sqlArgs.host_name = "root";
	sqlArgs.host_password = "1234";
	sqlArgs.db_name = "sign";
	sqlArgs.port = 3306;
	if (false == SQL::SQLManager::StaticInit())
		return false;
	SQL::SQLManager::sInstance->Initialize(&sqlArgs);
	// SQL end

	// Sign Manager
	if (false == Sign::SignManager::StaticInit())
		return false;
	Sign::SignManager::sInstance->Initialize(nullptr);
	// Sign Manager end

#ifdef  __DEBUG
	printf("----- Engine::Initialize() Complete -----\n");
#endif //  __DEBUG

	return true;
}

void Engine::Finalize() noexcept
{
#ifdef  __DEBUG
	printf("----- Engine::Finalize() Complete -----\n");
#endif //  __DEBUG

	// base finalize
	IOCP_Base::EngineBase<Engine, IOCPSession>::Finalize();

#ifdef  __DEBUG
	printf("----- Engine::Finalize() Complete -----\n");
#endif //  __DEBUG
}

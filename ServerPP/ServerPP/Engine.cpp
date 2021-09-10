#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Engine);

bool Engine::DoFrame()
{

	return true;
}

bool Engine::Initialize(LPVOID args) noexcept
{
	IOCP_Base::EngineBase<Engine, IOCPSession>::Initialize(args);

	SQL::SQLManager::InitArgs sqlArgs;
	sqlArgs.ip = "49.1.218.29";
	sqlArgs.host_name = "root";
	sqlArgs.host_password = "1234";
	sqlArgs.db_name = "sign";
	sqlArgs.port = 3306;
	if (false == SQL::SQLManager::StaticInit())
		return false;
	SQL::SQLManager::sInstance->Initialize(&sqlArgs);

	if (false == Sign::SignManager::StaticInit())
		return false;
	Sign::SignManager::sInstance->Initialize(nullptr);

	return true;
}

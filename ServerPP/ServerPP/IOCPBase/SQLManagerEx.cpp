#include "IOCPBase_RootHeader.h"

Implementation_sInstance(SQLManagerEx);

bool SQLManagerEx::Initialize(LPVOID args) noexcept
{
	if (false == SQL::SQLManager::StaticInit())
		return false;
	if (false == SQL::SQLManager::sInstance->Initialize(args))
		return false;

	return true;
}

void SQLManagerEx::Finalize() noexcept
{
	bLoop = false;
	
	queue.Finalize();
}

void SQLManagerEx::Request(NetBase::SQLPacketPtr queryPacket)
{
	queue.Push(queryPacket);
}

DWORD __stdcall SQLManagerEx::DBThread(LPVOID arg)
{
	while (SQLManagerEx::sInstance->bLoop)
	{
		queue.Wait();

		auto pQuery = queue.Pop();
		SQL::SQLManager::sInstance->Query(pQuery->query, pQuery->query_result);
		
		// TODO : Post Queue ÇØ¾ßµÇ
	}
	return 0;
}
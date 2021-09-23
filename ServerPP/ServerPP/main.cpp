
//#define __CIPHER_ON

#include "IOCPNet_RootHeader.h"

int main()
{
	if (false == Utils::StringUtil::SetLocale())
		return 1;

	if (false == Engine::StaticInit())
		return 1;

	Engine::InitArgs args;
	args.stream_buf_size = 1024;
	args.acptpacket_pool_size = 1024;
	args.recvpacket_pool_size = 1024;
	args.sendpacket_pool_size = 1024;

	if (false == Engine::sInstance->Initialize(&args))
		return 1;

	if (false == Engine::sInstance->DoLoop())
		return 1;

	Engine::sInstance->Finalize();

	return 0;
}
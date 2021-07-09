#include "base.h"


int main()
{
	if (false == NetworkManagerClient::StaticInit())
		return 1;

	while (true)
	{
		if (false == NetworkManagerClient::sInstance->DoFrame())
			return false;
	}

	NetworkManagerClient::sInstance->Finalize();

	return 0;
}
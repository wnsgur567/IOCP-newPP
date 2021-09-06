#include "base.h"

Implementation_sInstance(NetworkEngine);
//
//bool NetworkEngine::DoFrame()
//{
//	if (false == NetworkManagerClient::sInstance->DoFrame())
//		return false;
//	return true;
//}
//
//bool NetworkEngine::DoLoop()
//{
//	while (true)
//	{
//		if (false == DoFrame())
//			break;
//	}
//
//	return true;
//}
//
//bool NetworkEngine::Initialize()
//{
//	if (false == SessionManager::StaticInit())
//		return false;
//	SessionManager::sInstance->RegistCreationFunction(SessionBase::CreateSession<ClientSession>);
//
//	if (false == NetworkManagerClient::StaticInit())
//		return false;
//
//	if (false == PacketManager::StaticInit())
//		return false;
//
//
//
//	return true;
//}
//
//void NetworkEngine::Finalize()
//{
//}

bool NetworkEngine::DoFrame()
{
	NetworkManagerClient::sInstance->DoFrame();

	return true;
}

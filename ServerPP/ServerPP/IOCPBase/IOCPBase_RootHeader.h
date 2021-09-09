#pragma once

#define __DEBUG

#include "../NetBase/NetBase_RootHeader.h"
#include "../Utils/KISA_CIPHER/CipherManager.h"

#include <unordered_set>

//constexpr unsigned __int16	SERVERPORT = 9000;
//constexpr unsigned __int32	BUFSIZE = 512;
//constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;

#include "SessionBase.h"
#include "IOCPSessionBase.h"
#include "IOCPSessionManager.h"

#include "NetworkManagerServer.h"
#include "IOCPNetworkManager.h"

#include "EngineBase.h"
#pragma once

#pragma comment(lib,"ws2_32")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<algorithm>
#include<stack>
#include<queue>
#include<chrono>

#undef max
using gid_t = size_t;
using gsize_t = size_t;

constexpr u_short			SERVERPORT = 9000;
constexpr unsigned __int32	BUFSIZE = 512;
constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;
#define SOCKET_END	0


using HandlePtr = std::shared_ptr<HANDLE>;


enum class ESessionState
{
	None,

	Sign,

	Disconnected
};



// base
#include "Singleton.h"
#include "CriticalSection.h"
//#include "KISA_CIPHER/KISA_SEED_ECB.h"
#include "CipherManager.h"

// net base
#include "MemoryStream.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "SocketUtil.h"


// IOCP packet and stream
#include "IOCPMemoryStream.h"
#include "Packet.h"
#include "PacketManager.h"

#include "SessionBase.h"
#include "SessionManagerBase.h"

#include "IOCPSessionBase.h"

#include "NetworkManagerServer.h"
#include "IOCPNetworkManagerBase.h"

#include "EngineBase.h"


// network
#include "SessionManager.h"
#include "IOCPSession.h"
#include "IOCPNetworkManager.h"
#include "Engine.h"

// app
#include "SignInfo.h"
#include "SignManager.h"
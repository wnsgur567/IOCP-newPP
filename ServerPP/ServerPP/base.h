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
using gid32_t = unsigned __int32;
using gid64_t = unsigned __int64;
using gsize32_t = unsigned __int32;
using gsize64_t = unsigned __int64;

constexpr u_short			SERVERPORT = 9000;
constexpr unsigned __int32	BUFSIZE = 512;
constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;
#define SOCKET_END	0

using HandlePtr = std::shared_ptr<HANDLE>;
using VoidPtr = std::shared_ptr<void>;

#define __DEBUG
#define __CIPHER_ON

// base
#include "Singleton.h"
#include "CriticalSection.h"
#include "CipherManager.h"


#include "SignInfo.h"
#include "SignManager.h"

// net base
#include "MemoryStream.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "SocketUtil.h"

// IOCP packet and stream
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

#include "ClientState.h"
#include "SignState.h"

#include "IOCPNetworkManager.h"
#include "Engine.h"

// app

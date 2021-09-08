#pragma once

#pragma comment(lib,"ws2_32")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<chrono>
#include<memory>
#include<vector>
#include<list>
#include<queue>

#undef max
using gid32_t = unsigned __int32;
using gid64_t = unsigned __int64;
using gsize32_t = unsigned __int32;
using gsize64_t = unsigned __int64;

using VoidPtr = std::shared_ptr<void>;
using HandlePtr = std::shared_ptr<HANDLE>;
#define SOCKET_END	0

#include "SocketAddress.h"
#include "TCPSocket.h"
#include "SocketUtil.h"

#include "MemoryStream.h"
#include "Packet.h"
#include "../Utils/Singleton.h"
#include "PacketManager.h"
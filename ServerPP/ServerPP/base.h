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
using Byte = char;
using id_t = unsigned __int32;

constexpr u_short			SERVERPORT = 9000;
constexpr unsigned __int32	BUFSIZE = 512;
constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;
#define SOCKET_END	0


using HandlePtr = std::shared_ptr<HANDLE>;

class TCPSocket;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;

class InputMemoryStream;
class OutputMemoryStream;
using InputMemoryStreamPtr = std::shared_ptr<InputMemoryStream>;
using OutputMemoryStreamPtr = std::shared_ptr<OutputMemoryStream>;

class PacketBase;
class AcceptPacket;
class RecvPacket;
class SendPacket;
using PacketBaseWeakPtr = std::weak_ptr<PacketBase>;
using PacketBasePtr = std::shared_ptr<PacketBase>;
using AcceptPacketPtr = std::shared_ptr<AcceptPacket>;
using RecvPacketPtr = std::shared_ptr<RecvPacket>;
using SendPacketPtr = std::shared_ptr<SendPacket>;

class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;

class SignInfo;
using SignInfoPtr = std::shared_ptr<SignInfo>;

enum class E_PacketState
{
	Error = -2,		// 비정상 종료
	End = -1,		// 정상 종료

	Idle = 0,
	InComplete,		// 미완성, 데이터 받는(전송) 중
	Completed,		// 완성
};

enum class E_OverlappedType
{
	Accept,
	Recv,
	Send,
};


#include "Singleton.h"
#include "AppSingleton.h"
#include "CriticalSection.h"

#include "MemoryStream.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "PacketManager.h"
#include "Packet.h"

#include "NetworkManager.h"

#include "SessionManager.h"
#include "Session.h"
#include "Engine.h"

#include "SignManager.h"
#include "SignInfo.h"
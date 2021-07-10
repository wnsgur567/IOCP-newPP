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
#define						SERVERIP "127.0.0.1"
constexpr u_short			SERVERPORT = 9000;
constexpr unsigned __int32	BUFSIZE = 512;
constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;
#define SOCKET_END	0


class TCPSocket;
class InputMemoryStream;
class OutputMemoryStream;
class PacketBase;
class AcceptPacket;
class RecvPacket;
class SendPacket;
class ClientSession;

using HandlePtr = std::shared_ptr<HANDLE>;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;
using InputMemoryStreamPtr = std::shared_ptr<InputMemoryStream>;
using OutputMemoryStreamPtr = std::shared_ptr<OutputMemoryStream>;
using PacketBaseWeakPtr = std::weak_ptr<PacketBase>;
using PacketBasePtr = std::shared_ptr<PacketBase>;
using AcceptPacketPtr = std::shared_ptr<AcceptPacket>;
using RecvPacketPtr = std::shared_ptr<RecvPacket>;
using SendPacketPtr = std::shared_ptr<SendPacket>;
using ClientSessionPtr = std::shared_ptr<ClientSession>;

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

#include "CriticalSection.h"
#include "Singleton.h"

#include "MemoryStream.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "PacketManager.h"
#include "Packet.h"

#include "NetworkManager.h"
#include "Session.h"

#include "NetworkEngine.h"
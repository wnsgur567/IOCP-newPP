#pragma once


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



constexpr unsigned __int16	SERVERPORT = 9000;
constexpr unsigned __int32	BUFSIZE = 512;
constexpr unsigned __int32	STREAMPOOLCAPACITY = 128;




#define __DEBUG
#define __CIPHER_ON

// base



#include "SignInfo.h"
#include "SignManager.h"

// net base


// IOCP packet and stream



// network
#include "IOCPSession.h"

#include "ClientState.h"
#include "SignState.h"

#include "Engine.h"

// app

#pragma once

//#pragma comment(lib,"ws2_32")

#include "IOCPBase/IOCPBase_RootHeader.h"

using ProtocolSize_t = __int64;
using ResultSize_t = __int32;

#include "IOCPSession.h"

#include "Utils/StringUtil.h"
#include "Utils/FileUtil.h"
#include "Utils/MySQL/SQLManager.h"

#include <map>
#include <unordered_map>

#include "SignInfo.h"
#include "SignManager.h"
#include "SignState.h"

#include "CharacterInfo.h"
#include "CharacterSelectManager.h"
#include "CharacterSelectState.h"

#include "NetGameObject.h"

#include "Vector2.h"
#include "Vector3.h"
#include "PlayerInfo.h"

#include "Sector.h"

#include "VillageInfoBase.h"
#include "VillageManager.h"
#include "VillageState.h"



#include "Engine.h"


#pragma once

//#pragma comment(lib,"ws2_32")

#include "IOCPBase/IOCPBase_RootHeader.h"
#include <unordered_map>
using ProtocolSize_t = uint64_t;
using ResultSize_t = uint32_t;

#include "NetGameObject.h"
#include "NetGameObjectManager.h"

#include "Vector2.h"
#include "Vector3.h"
#include "CharacterInfo.h"
#include "PlayerInfo.h"

#include "Sector.h"
#include "SectorManager.h"

#include "VillageInfoBase.h"
#include "FirtstVillage.h"

#include "IOCPSession.h"

#include "Utils/StringUtil.h"
#include "Utils/FileUtil.h"
#include "Utils/MySQL/SQLManager.h"

#include <map>
#include <unordered_map>

#include "SignInfo.h"
#include "SignManager.h"
#include "SignState.h"


#include "CharacterSelectManager.h"
#include "CharacterSelectState.h"

#include "PlayerPartyInfo.h"
#include "PlayerPartyManager.h"


#include "VillageManager.h"

#include "VillageState.h"

#include "DungeonState.h"


#include "Engine.h"


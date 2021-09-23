#include "IOCPNet_RootHeader.h"

// 1 2 3
// 4 5 6
// 7 8 9
// param pos 는 좌상단 기준 절대 pixel position
Sector::EDirection Sector::GetRelativeDirInSector(const Vector3& pos)
{	// unity 기준 y가 점프
	// x check

	int x_flag = 0;
	int x_left = m_left_top_pixel_position.x;
	if (pos.x < x_left)
		x_flag = -1;
	else if (x_left + m_size.x < pos.x)
		x_flag = 1;

	// z check
	int z_flag = 0;
	int z_top = m_left_top_pixel_position.y;
	if (pos.z < z_top)
		z_flag = -1;
	else if (z_top + m_size.y < pos.z)
		z_flag = 1;


	if (x_flag == -1 && z_flag == -1)
		return EDirection::LeftUpper;
	if (x_flag == 0 && z_flag == -1)
		return EDirection::MiddleUpper;
	if (x_flag == 1 && z_flag == -1)
		return EDirection::RightUpper;
	if (x_flag == -1 && z_flag == 0)
		return EDirection::LeftMiddle;
	if (x_flag == 0 && z_flag == 0)
		return EDirection::Middle;
	if (x_flag == 1 && z_flag == 0)
		return EDirection::RightMiddle;
	if (x_flag == -1 && z_flag == 1)
		return EDirection::LeftLower;
	if (x_flag == 0 && z_flag == 1)
		return EDirection::MiddleLower;
	//if (x_flag == 1 && z_flag == 1)
	return EDirection::RightLower;
}

void Sector::Initialize(Vector2Int pos, Vector2Int grid, Vector2Int size, bool activate)
{
	m_left_top_pixel_position = pos;
	m_grid_position = grid;
	m_size = size;
	IsAccesible = activate;

#ifdef __DEBUG
	// TODO : Test끝나면 삭제하기
	// sector 마다 dummy player 생성하기 (Test용도임)
	auto obj = NetGameObjectManager::sInstance->Create<PlayerInfo>();

	// dummy info
	// sector 의 중앙마다 하나씩 dummy를 배치
	int obj_x = m_left_top_pixel_position.x + m_size.x / 2;
	int obj_y = -m_left_top_pixel_position.y - m_size.y / 2;
	CharacterInfoPtr char_info = std::make_shared<CharacterInfo>();
	std::wstring dummy_name = L"더미더미";
	char_info->SetInfo(1U, 10000U, 1U, dummy_name);
	obj->SetCharacterInfo(char_info);
	obj->SetPosition(static_cast<float>(obj_x), static_cast<float>(obj_y), 0.f);
	m_player_map.insert({ obj->GetNetID(), obj });
#endif 
}

// after initilaized
void Sector::SetNearSector(const std::vector<std::vector<SectorPtr>>& sectors)
{
	Vector2Int offset[3][3] = {
		{{-1,-1},{0,-1},{1,-1}},
		{{-1,0},{0,0},{1,0}},
		{{-1,1},{0,1},{1,1}}
	};

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			// except me
			if (i == 1 && j == 1)
				continue;
			int row = m_grid_position.x + offset[i][j].x;
			int col = m_grid_position.y + offset[i][j].y;

			// out of range
			if (row < 0 || col < 0)
				continue;
			if (row >= sectors.size() || col >= sectors[0].size())	// 사각으로 꽉 차있는 경우만 가능
				continue;

			// near sector
			if (sectors[row][col]->IsAccesible)
				m_nearSector_vec.push_back(sectors[row][col]);
		}
	}
}

void Sector::Finalize()
{
}

// 좌상단이 0,0 기준
bool Sector::IsInSector(const Vector2& vec)
{
	Vector2Int left_top = m_left_top_pixel_position;
	Vector2Int right_bottom = { m_left_top_pixel_position.x + m_size.x,
								m_left_top_pixel_position.y + m_size.y };

	int x = static_cast<int>(vec.x);
	int y = static_cast<int>(vec.y);
	if (x < left_top.x || right_bottom.x < x)
		return false;
	if (y < left_top.y || right_bottom.y < y)
		return false;
	return true;
}

bool Sector::IsInSector(const Vector2Int& vec)
{
	Vector2Int left_top = m_left_top_pixel_position;
	Vector2Int right_bottom = { m_left_top_pixel_position.x + m_size.x,
								m_left_top_pixel_position.y + m_size.y };

	int x = vec.x;
	int y = vec.y;
	if (x < left_top.x || right_bottom.x < x)
		return false;
	if (y < left_top.y || right_bottom.y < y)
		return false;
	return true;
}

void Sector::PlayerMove(PlayerInfoPtr inpPlayer)
{
	auto pos = inpPlayer->GetPosition();
	auto dir = GetRelativeDirInSector(pos);

	switch (dir)
	{
	case Sector::EDirection::Middle:

		break;

	case Sector::EDirection::LeftUpper:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::MiddleUpper:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::RightUpper:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::LeftMiddle:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::RightMiddle:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::LeftLower:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::MiddleLower:
		LeaveSection(inpPlayer);
		break;
	case Sector::EDirection::RightLower:
		LeaveSection(inpPlayer);
		break;
	default:
		// 예외처리
		break;
	}
}

// 시야 내 모든 플레이어 긁어오기
void Sector::GetNearPlayerInfos(std::vector<PlayerInfoPtr>& outVec)
{
	outVec.clear();

	for (auto& item : m_player_map)
	{
		outVec.push_back(item.second);
	}

	for (auto& nearSector : m_nearSector_vec)
	{
		for (auto& item : nearSector->m_player_map)
		{
			outVec.push_back(item.second);
		}
	}
}



void Sector::EnterSection(PlayerInfoPtr inpPlayer)
{
	m_player_map.insert({ inpPlayer->GetNetID(), inpPlayer });
}

void Sector::LeaveSection(PlayerInfoPtr inpPlayer)
{
	m_player_map.erase(inpPlayer->GetNetID());
}

// 주변에 이 섹터가 시야 범위 내인 모든 근처 섹터의 player 들에게 전송
void Sector::SendAll(NetBase::OutputMemoryStreamPtr inpStream)
{
	std::vector<PlayerInfoPtr> players;
	GetNearPlayerInfos(players);

	for (auto& item : m_player_map)
	{
		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		(*sendstream) = (*inpStream);	// copy
		auto current_session = item.second->GetSession();

#ifdef __DEBUG
		// DEBUG 모드에서의 Dummy player 일 경우 session 할당 안되있음
		if (current_session != nullptr)
			current_session->Send(sendstream);
		else
		{
			NetBase::PacketManager::sInstance->RetrieveSendStream(sendstream);
		}
#else
		current_session->Send(sendstream);
#endif
		}

	for (auto& item : players)
	{
		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		(*sendstream) = (*inpStream);	// copy
		auto current_session = item->GetSession();

#ifdef __DEBUG
		// DEBUG 모드에서의 Dummy player 일 경우 session 할당 안되있음
		if (current_session != nullptr)
			current_session->Send(sendstream);
		else
		{
			NetBase::PacketManager::sInstance->RetrieveSendStream(sendstream);
		}
#else
		current_session->Send(sendstream);
#endif	
		}

	NetBase::PacketManager::sInstance->RetrieveSendStream(inpStream);
	}


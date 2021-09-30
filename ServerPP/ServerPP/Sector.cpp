#include "IOCPNet_RootHeader.h"

// 1 2 3
// 4 5 6
// 7 8 9
// param pos �� �»�� ���� ���� pixel position
Sector::EDirection Sector::GetRelativeDirInSector(const Vector3& pos)
{	// y �� -�� �Ѿ�� (unity)
	// x check

	int x_flag = 0;
	int x_left = m_left_top_pixel_position.x;
	if (pos.x < x_left)
		x_flag = -1;
	else if (x_left + m_size.x < pos.x)
		x_flag = 1;

	// z check
	int y_flag = 0;
	int y_top = m_left_top_pixel_position.y;
	if (-pos.y < y_top)
		y_flag = -1;
	else if (y_top + m_size.y < -pos.y)
		y_flag = 1;


	if (x_flag == -1 && y_flag == -1)
		return EDirection::LeftUpper;
	if (x_flag == 0 && y_flag == -1)
		return EDirection::MiddleUpper;
	if (x_flag == 1 && y_flag == -1)
		return EDirection::RightUpper;
	if (x_flag == -1 && y_flag == 0)
		return EDirection::LeftMiddle;
	if (x_flag == 0 && y_flag == 0)
		return EDirection::Middle;
	if (x_flag == 1 && y_flag == 0)
		return EDirection::RightMiddle;
	if (x_flag == -1 && y_flag == 1)
		return EDirection::LeftLower;
	if (x_flag == 0 && y_flag == 1)
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
	// TODO : Test������ �����ϱ�
	// sector ���� dummy player �����ϱ� (Test�뵵��)
	auto obj = NetGameObjectManager::sInstance->Create<PlayerInfo>();

	// dummy info
	// sector �� �߾Ӹ��� �ϳ��� dummy�� ��ġ
	int obj_x = m_left_top_pixel_position.x + m_size.x / 2;
	int obj_y = -m_left_top_pixel_position.y - m_size.y / 2;
	CharacterInfoPtr char_info = std::make_shared<CharacterInfo>();
	std::wstring dummy_name = L"���̴���";
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

	m_nearSector_vec.resize(3);
	for (size_t i = 0; i < 3; i++)
	{
		m_nearSector_vec[i].resize(3);
		for (size_t j = 0; j < 3; j++)
		{
			// except me
			if (i == 1 && j == 1)
			{
				m_nearSector_vec[1][1] = shared_from_this();
				continue;
			}
			int row = m_grid_position.x + offset[i][j].x;
			int col = m_grid_position.y + offset[i][j].y;

			// out of range
			if (row < 0 || col < 0)
			{
				m_nearSector_vec[i][j] = nullptr;
				continue;
			}
			// �簢���� �� ���ִ� ��츸 ����
			if (row >= sectors.size() || col >= sectors[0].size())
			{
				m_nearSector_vec[i][j] = nullptr;
				continue;
			}

			// near sector
			if (sectors[row][col]->IsAccesible)
				m_nearSector_vec[i][j] = sectors[row][col];
			else
				m_nearSector_vec[i][j] = nullptr;
		}
	}
}

void Sector::Finalize()
{
}

// �»���� 0,0 ����
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


// �þ� �� ��� �÷��̾� �ܾ����
void Sector::GetNearPlayerInfos(std::vector<PlayerInfoPtr>& outVec)
{
	outVec.clear();

	for (auto& nearSectorVec : m_nearSector_vec)
	{
		for (auto& innerSectorVec : nearSectorVec)
		{
			if (innerSectorVec != nullptr)
				for (auto& item : innerSectorVec->m_player_map)
				{
					outVec.push_back(item.second);
				}
		}
	}
}


// ���� ����� 
// Ư�� sector���� ���� ������ ���
void Sector::FirstEnterSection(PlayerInfoPtr inpPlayer)
{
	inpPlayer->SetSector(shared_from_this());

	{	// player ���ο��� �ڽ��� ������ ������
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		SectorManager::EProtocol protocol = SectorManager::EProtocol::FirstInit;
		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
		write_size += NetBase::WriteToBinStream(stream, inpPlayer);
#ifdef __DEBUG	
		printf("SectorManager FirstInit  write to stream : %dbytes\n", write_size);
#endif
		auto pSession = inpPlayer->GetSession();
		pSession->Send(stream);
	}

	{	// ��ó player ���� ���� �߰��� player ���� ������	

		// ���� �÷��̾� ���� stream�� write
		// protocol : enter in view
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		SectorManager::EProtocol protocol = SectorManager::EProtocol::EnterInView;
		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
		write_size += NetBase::WriteToBinStream(stream, inpPlayer);
#ifdef __DEBUG	
		printf("SectorManager FirstInit  write to stream : %dbytes\n", write_size);
#endif

		// ��ó �÷��̾�� ���� player �� ������ ����
		SendAll(stream);
	}

	// ���Ϳ� ���� �÷��̾� ���� ptr �߰�
	m_player_map.insert({ inpPlayer->GetNetID(), inpPlayer });
}

void Sector::MoveInSection(PlayerInfoPtr inpPlayer)
{
	auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
	SectorManager::EProtocol protocol = SectorManager::EProtocol::PlayerMove;
	int write_bytes = 0;
	write_bytes += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
	write_bytes += NetBase::WriteToBinStream(stream, inpPlayer->GetNetID());
	Vector3 player_pos = inpPlayer->GetPosition();	
	write_bytes += NetBase::WriteToBinStream(stream, player_pos);

#ifdef __DEBUG
	printf("SectorManager MoveProcess() write size : %d\n", write_bytes);
#endif // __DEBUG
	SendAllExceptOne(stream, inpPlayer->GetNetID());
}

// Ư�� �������� �̵��Ͽ� sector �� �����ϴ� ���
void Sector::EnterSection(PlayerInfoPtr inpPlayer, EDirection inPlayerMoveDir)
{
	inpPlayer->SetSector(shared_from_this());

	// view �� player �� �߰� �� ���� �̾ƿ���
	std::vector<SectorPtr> outViewSectors;
	switch (inPlayerMoveDir)
	{
	case Sector::EDirection::LeftUpper:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[0][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][1]);
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][0]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		break;
	case Sector::EDirection::MiddleUpper:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[0][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][1]);
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		break;
	case Sector::EDirection::RightUpper:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[0][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][1]);
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][2]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::LeftMiddle:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[1][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][0]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		break;
	case Sector::EDirection::RightMiddle:
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][2]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::LeftLower:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[1][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][0]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		if (m_nearSector_vec[2][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][1]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::MiddleLower:
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		if (m_nearSector_vec[2][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][1]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::RightLower:
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][2]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		if (m_nearSector_vec[2][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][1]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		break;
	}

	{	// player info ������
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		SectorManager::EProtocol protocol = SectorManager::EProtocol::EnterInView;
		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
		write_size += NetBase::WriteToBinStream(stream, inpPlayer);
#ifdef __DEBUG	
		printf("Sector EnterSection write to stream : %dbytes\n", write_size);
#endif
		for (auto& s : outViewSectors)
		{	// �̾ƿ� ���� ���ο� �ִ� �÷��̾�鿡�� ��� ����
			s->SendAllInSector(stream);
		}
	}


	{	// ���� �÷��̾�� ���ο� ���� ������ �ִ� player info ������
		// ���� �˻��� �÷��̾� ������ ���� vector
		std::vector<PlayerInfoPtr> newPlayers;
		for (auto& s : outViewSectors)
		{
			for (auto& item : s->m_player_map)
			{
				newPlayers.push_back(item.second);
			}
		}
		// 10�� ������ ��� send
		int start_index = 0;
		size_t nearPlayer_count = newPlayers.size();
		while (nearPlayer_count)
		{
			int cur_loopsize;
			if (nearPlayer_count > 10)
			{
				cur_loopsize = 10;
				nearPlayer_count -= 10;
			}
			else
			{
				cur_loopsize = static_cast<int>(nearPlayer_count);
				nearPlayer_count = 0;
			}

			int write_size = 0;
			SectorManager::EProtocol protocol = SectorManager::EProtocol::EnterSection;
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// write protocol
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
			// write otherPlayerInfo count			
			write_size += NetBase::WriteToBinStream(stream, cur_loopsize);
			// write other player info
			for (int i = start_index; i < cur_loopsize; i++)
			{
				write_size += NetBase::WriteToBinStream(stream, newPlayers[i]);
			}
#ifdef __DEBUG	
			printf("Sector EnterSection write to stream : %dbytes\n", write_size);
#endif
			// ���� �÷��̾�� other player ���� send
			inpPlayer->GetSession()->Send(stream);
		}
	}

	// ���Ϳ� ���� �÷��̾� ���� ptr �߰�
	m_player_map.insert({ inpPlayer->GetNetID(),inpPlayer });
}

// Ư�� �������� �̵��Ͽ� sector���� ������ ��� ȣ��
void Sector::LeaveSection(PlayerInfoPtr inpPlayer, EDirection inPlayerMoveDir)
{
	// view ���� ����� ���� �̾ƿ���
	std::vector<SectorPtr> outViewSectors;
	switch (inPlayerMoveDir)
	{
	case Sector::EDirection::LeftUpper:
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][2]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		if (m_nearSector_vec[2][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][1]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		break;
	case Sector::EDirection::MiddleUpper:
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		if (m_nearSector_vec[2][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][1]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::RightUpper:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[1][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][0]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		if (m_nearSector_vec[2][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][1]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::LeftMiddle:
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][2]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::RightMiddle:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[1][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][0]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		break;
	case Sector::EDirection::LeftLower:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[0][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][1]);
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][2]);
		if (m_nearSector_vec[2][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][2]);
		break;
	case Sector::EDirection::MiddleLower:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[0][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][1]);
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		break;
	case Sector::EDirection::RightLower:
		if (m_nearSector_vec[0][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][0]);
		if (m_nearSector_vec[0][1] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][1]);
		if (m_nearSector_vec[0][2] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[0][2]);
		if (m_nearSector_vec[1][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[1][0]);
		if (m_nearSector_vec[2][0] != nullptr)
			outViewSectors.push_back(m_nearSector_vec[2][0]);
		break;
	}

	// ���� ���Ϳ��� player ����
	inpPlayer->SetSector(nullptr);
	m_player_map.erase(inpPlayer->GetNetID());

	// net object id �� ������ Ȯ�� ����!
	{	// view ������ ���� �ٸ� ����鿡�� �ڽ��� id ������
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		SectorManager::EProtocol protocol = SectorManager::EProtocol::LeaveInView;
		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
		write_size += NetBase::WriteToBinStream(stream, inpPlayer->GetNetID());

#ifdef __DEBUG	
		printf("Sector LeaveSection write to stream : %dbytes\n", write_size);
#endif

		for (auto& s : outViewSectors)
		{
			s->SendAllInSector(stream);
		}
	}

	{	// ���ο��Լ� ����� section �� ������Ʈ���� ���� ������
		// ���� �˻��� �÷��̾� ������ ���� vector
		std::vector<PlayerInfoPtr> newPlayers;
		for (auto& s : outViewSectors)
		{
			for (auto& item : s->m_player_map)
			{
				newPlayers.push_back(item.second);
			}
		}
		// 10�� ������ ��� send
		int start_index = 0;
		size_t nearPlayer_count = newPlayers.size();
		while (nearPlayer_count)
		{
			int cur_loopsize;
			if (nearPlayer_count > 10)
			{
				cur_loopsize = 10;
				nearPlayer_count -= 10;
			}
			else
			{
				cur_loopsize = static_cast<int>(nearPlayer_count);
				nearPlayer_count = 0;
			}

			int write_size = 0;
			SectorManager::EProtocol protocol = SectorManager::EProtocol::LeaveSection;
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// write protocol
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
			// write otherPlayerInfo count			
			write_size += NetBase::WriteToBinStream(stream, cur_loopsize);
			// write other player info
			for (int i = start_index; i < cur_loopsize; i++)
			{
				write_size += NetBase::WriteToBinStream(stream, newPlayers[i]->GetNetID());
			}
#ifdef __DEBUG	
			printf("Sector EnterSection write to stream : %dbytes\n", write_size);
#endif
			// ���� �÷��̾�� other player ���� send
			inpPlayer->GetSession()->Send(stream);
		}
	}
}

// sector���� �ƿ� ����� ���
// ���� Ȥ�� ������ �̵� ��
void Sector::DisapearSection(PlayerInfoPtr inpPlayer)
{
	inpPlayer->SetSector(nullptr);
	m_player_map.erase(inpPlayer->GetNetID());

	// net object id �� ������ Ȯ�� ����!
	{
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		SectorManager::EProtocol protocol = SectorManager::EProtocol::LeaveInView;
		int write_size = 0;
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
		auto netobj_id = inpPlayer->GetNetID();
		write_size += NetBase::WriteToBinStream(stream, netobj_id);
#ifdef __DEBUG	
		printf("Sector DisapearSection  write to stream : %dbytes\n", write_size);
#endif

		// 
		SendAll(stream);
	}
}

// ���� �÷��̾� ������ player�鿡�� ����
// ���� �÷��̾�� ���� ���� ���Ϳ����� �˻�...
void Sector::SendAllExceptOne(NetBase::OutputMemoryStreamPtr inpStream, uint64_t net_id)
{
	std::vector<PlayerInfoPtr> players;
	GetNearPlayerInfos(players);
		
//	for (auto& item : m_player_map)
//	{
//		if (item.first == net_id)
//			continue;
//
//		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
//		(*sendstream) = (*inpStream);	// copy
//		auto current_session = item.second->GetSession();
//
//#ifdef __DEBUG
//		// DEBUG ��忡���� Dummy player �� ��� session �Ҵ� �ȵ�����
//		if (current_session != nullptr)
//			current_session->Send(sendstream);
//		else
//		{
//			NetBase::PacketManager::sInstance->RetrieveSendStream(sendstream);
//		}
//#else
//		current_session->Send(sendstream);
//#endif
//		}

	for (auto& item : players)
	{
		if (item->GetNetID() == net_id)
			continue;
		
		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		(*sendstream) = (*inpStream);	// copy
		auto current_session = item->GetSession();

#ifdef __DEBUG
		// DEBUG ��忡���� Dummy player �� ��� session �Ҵ� �ȵ�����
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

// �ֺ��� �� ���Ͱ� �þ� ���� ���� ��� ��ó ������ player �鿡�� ����
void Sector::SendAll(NetBase::OutputMemoryStreamPtr inpStream)
{
	std::vector<PlayerInfoPtr> players;
	GetNearPlayerInfos(players);

//	for (auto& item : m_player_map)
//	{
//		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
//		(*sendstream) = (*inpStream);	// copy
//		auto current_session = item.second->GetSession();
//
//#ifdef __DEBUG
//		// DEBUG ��忡���� Dummy player �� ��� session �Ҵ� �ȵ�����
//		if (current_session != nullptr)
//			current_session->Send(sendstream);
//		else
//		{
//			NetBase::PacketManager::sInstance->RetrieveSendStream(sendstream);
//		}
//#else
//		current_session->Send(sendstream);
//#endif
//		}

	for (auto& item : players)
	{
		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		(*sendstream) = (*inpStream);	// copy
		auto current_session = item->GetSession();

#ifdef __DEBUG
		// DEBUG ��忡���� Dummy player �� ��� session �Ҵ� �ȵ�����
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

// ���� ���Ϳ� �ִ� ����鿡�Ը� ������
void Sector::SendAllInSector(NetBase::OutputMemoryStreamPtr inpStream)
{
	for (auto& item : m_player_map)
	{
		auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		(*sendstream) = (*inpStream);	// copy
		auto current_session = item.second->GetSession();

#ifdef __DEBUG
		// DEBUG ��忡���� Dummy player �� ��� session �Ҵ� �ȵ�����
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
	}


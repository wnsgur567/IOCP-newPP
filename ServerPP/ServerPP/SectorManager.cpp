#include "IOCPNet_RootHeader.h"

Implementation_sInstance(SectorManager);

bool SectorManager::Initialize(LPVOID) noexcept
{
	return true;
}

void SectorManager::Finalize() noexcept
{
}

void SectorManager::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{
	if ((ProtocolSize_t)EProtocol::PlayerAction & inOrigin)
	{
		if ((ProtocolSize_t)EProtocol::PlayerMove & inOrigin)
		{
			outProtocol = EProtocol::PlayerMoveAndAction;
			return;
		}
		outProtocol = EProtocol::PlayerAction;
		return;
	}
	if ((ProtocolSize_t)EProtocol::PlayerMove & inOrigin)
	{
		outProtocol = EProtocol::PlayerMove;
		return;
	}
}


SectorManager::EResult SectorManager::FirstEnterProcess(SectorPtr inCurrentSector, Vector2 inlocal_position, PlayerInfoPtr inpPlayer, IOCP_Base::IOCPSessionBasePtr inpSession)
{
	{	// �ֺ� �÷��̾� ������ CLIENT ���� ����
		// �ֺ� �÷��̾� ���� ��������
		std::vector<PlayerInfoPtr> nearPlayers;
		inCurrentSector->GetNearPlayerInfos(nearPlayers);

		// 10�� ������ ��� send
		int start_index = 0;
		size_t nearPlayer_count = nearPlayers.size();
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
			EProtocol protocol = EProtocol::FirstInit_Others;
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// write protocol
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
			// write otherPlayerInfo count			
			write_size += NetBase::WriteToBinStream(stream, cur_loopsize);
			// write other player info
			for (int i = start_index; i < cur_loopsize; i++)
			{
				write_size += NetBase::WriteToBinStream(stream, nearPlayers[i]);
			}
#ifdef __DEBUG	
			printf("SectorManager FirstInit_Others  write to stream : %dbytes\n", write_size);
#endif
			// ���� �÷��̾�� other player ���� send
			inpSession->Send(stream);
		}
		// ���� Ŭ���̾�Ʈ���� ��ó player info ���� ���� �Ϸ�
	}

	{	// ���� Ŭ���̾�Ʈ���� control player info ������ ����
		// ���� player ��ġ ����
		Vector2Int sector_pos = inCurrentSector->GetPixelPos();
		float x_pos = static_cast<float>(sector_pos.x) + inlocal_position.x;
		float y_pos = -static_cast<float>(sector_pos.y) - inlocal_position.y;
		inpPlayer->SetPosition(x_pos, y_pos, 0.f);

		// ���Ϳ� player ���
		// ���ο��� ������ ó���ϰ� ���ΰ� �ֺ�player���� �����ϰ�
		inCurrentSector->FirstEnterSection(inpPlayer);
	}

	return {};
}

SectorManager::EResult SectorManager::ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer)
{	// ���ο����� �ൿ�ϴ� ���� �ٲ� �� ����
	return {};
}


// 1. ���ο��� stream���� ��ġ �� �����ͼ�
// 2. player �̵���Ű�� 
// 3. �̵��� �ڸ��� ���� ���� ��ȭ��Ű��
SectorManager::EResult SectorManager::MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// ���� �ܺη� �̵� ���ɼ� ����

	// stream ���κ��� ���� player �� position �� ��������
	Vector3 player_pos;
	ISerializable* ptr = &player_pos;
	NetBase::ReadFromBinStream(inpStream, ptr);

	// �̵��� ��ġ ����ȭ
	inpPlayer->SetPosition(player_pos.x, player_pos.y, player_pos.z);
	printf("SectorManager MoveProcess() pos : (%0.2f,%0.2f,%0.2f)\n", player_pos.x, player_pos.y, player_pos.z);

	// ������ �ִ� ���͸� �������� �Ǻ�
	auto dir = inCurrent_sector->GetRelativeDirInSector(player_pos);

	// ����ȭ �� ��ġ���� �Ѹ���	
	switch (dir)
	{
	case Sector::EDirection::Middle:
	{	// ���� �̵����� ����

		inCurrent_sector->MoveInSection(inpPlayer);
		return {};
	}
	break;
	// ���� �̵���
	// ���� ���� �ʿ�
	case Sector::EDirection::LeftUpper:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[0][0];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::LeftUpper);
			newSector->EnterSection(inpPlayer, Sector::EDirection::LeftUpper);
		}
		return {};
	}
	break;

	case Sector::EDirection::MiddleUpper:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[0][1];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::MiddleUpper);
			newSector->EnterSection(inpPlayer, Sector::EDirection::MiddleUpper);
		}
		return {};
	}
	break;

	case Sector::EDirection::RightUpper:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[0][2];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::RightUpper);
			newSector->EnterSection(inpPlayer, Sector::EDirection::RightUpper);
		}
		return {};
	}
	break;
	case Sector::EDirection::LeftMiddle:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[1][0];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::LeftMiddle);
			newSector->EnterSection(inpPlayer, Sector::EDirection::LeftMiddle);
		}
		return {};
	}
	break;
	case Sector::EDirection::RightMiddle:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[1][2];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::RightMiddle);
			newSector->EnterSection(inpPlayer, Sector::EDirection::RightMiddle);
		}
		return {};
	}
	break;
	case Sector::EDirection::LeftLower:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[2][0];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::LeftLower);
			newSector->EnterSection(inpPlayer, Sector::EDirection::LeftLower);
		}
		return {};
	}
	break;
	case Sector::EDirection::MiddleLower:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[2][1];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::MiddleLower);
			newSector->EnterSection(inpPlayer, Sector::EDirection::MiddleLower);
		}
		return {};
	}
	break;
	case Sector::EDirection::RightLower:
	{
		// ������ ���ο� ����
		auto newSector = inCurrent_sector->m_nearSector_vec[2][2];
		if (newSector != nullptr)
		{
			// ���� ���Ϳ��� ����
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::RightLower);
			newSector->EnterSection(inpPlayer, Sector::EDirection::RightLower);
		}
		return {};
	}
	break;

	default:
		break;
	}

	return {};
}

SectorManager::EResult SectorManager::MoveAndActionProcess(std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// ���� �ܺη� �̵� ���ɼ� ����
	return {};
}


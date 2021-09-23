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


SectorManager::EResult SectorManager::FirstEnterProcess(SectorPtr inCurrentSector, Vector2 inlocal_position, PlayerInfoPtr inpPlayer ,IOCP_Base::IOCPSessionBasePtr inpSession)
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
			NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
			// write otherPlayerInfo count			
			NetBase::WriteToBinStream(stream, cur_loopsize);
			// write other player info
			for (int i = start_index; i < cur_loopsize; i++)
			{
				write_size = NetBase::WriteToBinStream(stream, nearPlayers[i]);
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
		inCurrentSector->EnterSection(inpPlayer);
		// player�� sector ���
		inpPlayer->SetSector(inCurrentSector);

		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		
		EProtocol protocol = EProtocol::FirstInit;
		int write_size = 0;
		write_size = NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
		write_size = NetBase::WriteToBinStream(stream, inpPlayer);
#ifdef __DEBUG	
		printf("SectorManager FirstInit  write to stream : %dbytes\n", write_size);
#endif
		inpSession->Send(stream);
	}

	// ������ �����ϴ� �ٸ� player�鿡�� Send �۾�
	{	// ���� Ŭ���̾�Ʈ�� player�� view ���� ���� ���� sector���� player �鿡�� ���ο� ������Ʈ�� �׸����� ����

		// �ֺ� ���Ϳ� �����ϴ� player ���� ���ο� player�� �˸�
		// ��ó ��� ������ �÷��̾�� ���� player ������ send
		// inCurrentSector->SendAll(stream);
	}
	return {};
}

SectorManager::EResult SectorManager::ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer)
{	// ���ο����� �ൿ�ϴ� ���� �ٲ� �� ����
	return {};
}

SectorManager::EResult SectorManager::MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// ���� �ܺη� �̵� ���ɼ� ����

	// stream ���κ��� ���� player �� position �� ��������

	auto player_pos = inpPlayer->GetPosition();
	// ������ �ִ� ���͸� �������� �Ǻ�
	auto dir = inCurrent_sector->GetRelativeDirInSector(player_pos);

	switch (dir)
	{
	case Sector::EDirection::Middle:
	{	// ���� �̵����� ����

	}
	break;
	// ���� �̵���
	// ���� ���� �ʿ�
	case Sector::EDirection::LeftUpper:
		break;
	case Sector::EDirection::MiddleUpper:
		break;
	case Sector::EDirection::RightUpper:
		break;
	case Sector::EDirection::LeftMiddle:
		break;
	case Sector::EDirection::RightMiddle:
		break;
	case Sector::EDirection::LeftLower:
		break;
	case Sector::EDirection::MiddleLower:
		break;
	case Sector::EDirection::RightLower:
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


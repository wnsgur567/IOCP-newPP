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
	{	// 주변 플레이어 정보를 CLIENT 에게 전송
		// 주변 플레이어 정보 가져오기
		std::vector<PlayerInfoPtr> nearPlayers;
		inCurrentSector->GetNearPlayerInfos(nearPlayers);

		// 10명 단위로 끊어서 send
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
			// 현재 플레이어에게 other player 정보 send
			inpSession->Send(stream);
		}
		// 현재 클라이언트에게 근처 player info 정보 전송 완료
	}

	{	// 현재 클라이언트에게 control player info 정보를 전송
		// 현재 player 위치 조정
		Vector2Int sector_pos = inCurrentSector->GetPixelPos();
		float x_pos = static_cast<float>(sector_pos.x) + inlocal_position.x;
		float y_pos = -static_cast<float>(sector_pos.y) - inlocal_position.y;
		inpPlayer->SetPosition(x_pos, y_pos, 0.f);

		// 섹터에 player 등록
		inCurrentSector->EnterSection(inpPlayer);
		// player에 sector 등록
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

	// 기존에 존재하는 다른 player들에게 Send 작업
	{	// 현재 클라이언트의 player가 view 범위 내로 들어온 sector들의 player 들에게 새로운 오브젝트를 그리도록 전송

		// 주변 섹터에 존재하는 player 에게 새로운 player를 알림
		// 근처 모든 섹터의 플레이어에게 현재 player 정보를 send
		// inCurrentSector->SendAll(stream);
	}
	return {};
}

SectorManager::EResult SectorManager::ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer)
{	// 내부에서만 행동하니 섹터 바뀔 일 없음
	return {};
}

SectorManager::EResult SectorManager::MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// 섹터 외부로 이동 가능성 있음

	// stream 으로부터 현재 player 의 position 값 가져오기

	auto player_pos = inpPlayer->GetPosition();
	// 기존에 있던 섹터를 기준으로 판별
	auto dir = inCurrent_sector->GetRelativeDirInSector(player_pos);

	switch (dir)
	{
	case Sector::EDirection::Middle:
	{	// 섹터 이동하지 않음

	}
	break;
	// 섹터 이동함
	// 섹터 갱신 필요
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
{	// 섹터 외부로 이동 가능성 있음
	return {};
}


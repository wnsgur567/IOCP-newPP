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
		// 내부에서 데이터 처리하고 본인과 주변player에게 전송하고
		inCurrentSector->FirstEnterSection(inpPlayer);
	}

	return {};
}

SectorManager::EResult SectorManager::ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer)
{	// 내부에서만 행동하니 섹터 바뀔 일 없음
	return {};
}


// 1. 내부에서 stream에서 위치 값 가져와서
// 2. player 이동시키고 
// 3. 이동한 자리에 대한 섹터 변화시키기
SectorManager::EResult SectorManager::MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer)
{	// 섹터 외부로 이동 가능성 있음

	// stream 으로부터 현재 player 의 position 값 가져오기
	Vector3 player_pos;
	ISerializable* ptr = &player_pos;
	NetBase::ReadFromBinStream(inpStream, ptr);

	// 이동된 위치 동기화
	inpPlayer->SetPosition(player_pos.x, player_pos.y, player_pos.z);
	printf("SectorManager MoveProcess() pos : (%0.2f,%0.2f,%0.2f)\n", player_pos.x, player_pos.y, player_pos.z);

	// 기존에 있던 섹터를 기준으로 판별
	auto dir = inCurrent_sector->GetRelativeDirInSector(player_pos);

	// 동기화 된 위치정보 뿌리기	
	switch (dir)
	{
	case Sector::EDirection::Middle:
	{	// 섹터 이동하지 않음

		inCurrent_sector->MoveInSection(inpPlayer);
		return {};
	}
	break;
	// 섹터 이동함
	// 섹터 갱신 필요
	case Sector::EDirection::LeftUpper:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[0][0];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::LeftUpper);
			newSector->EnterSection(inpPlayer, Sector::EDirection::LeftUpper);
		}
		return {};
	}
	break;

	case Sector::EDirection::MiddleUpper:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[0][1];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::MiddleUpper);
			newSector->EnterSection(inpPlayer, Sector::EDirection::MiddleUpper);
		}
		return {};
	}
	break;

	case Sector::EDirection::RightUpper:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[0][2];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::RightUpper);
			newSector->EnterSection(inpPlayer, Sector::EDirection::RightUpper);
		}
		return {};
	}
	break;
	case Sector::EDirection::LeftMiddle:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[1][0];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::LeftMiddle);
			newSector->EnterSection(inpPlayer, Sector::EDirection::LeftMiddle);
		}
		return {};
	}
	break;
	case Sector::EDirection::RightMiddle:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[1][2];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::RightMiddle);
			newSector->EnterSection(inpPlayer, Sector::EDirection::RightMiddle);
		}
		return {};
	}
	break;
	case Sector::EDirection::LeftLower:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[2][0];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::LeftLower);
			newSector->EnterSection(inpPlayer, Sector::EDirection::LeftLower);
		}
		return {};
	}
	break;
	case Sector::EDirection::MiddleLower:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[2][1];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
			inCurrent_sector->LeaveSection(inpPlayer, Sector::EDirection::MiddleLower);
			newSector->EnterSection(inpPlayer, Sector::EDirection::MiddleLower);
		}
		return {};
	}
	break;
	case Sector::EDirection::RightLower:
	{
		// 입장할 새로운 섹터
		auto newSector = inCurrent_sector->m_nearSector_vec[2][2];
		if (newSector != nullptr)
		{
			// 현재 섹터에서 퇴장
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
{	// 섹터 외부로 이동 가능성 있음
	return {};
}


#pragma once

class PlayerPartyManager : MyBase::Singleton<PlayerPartyManager>
{
	friend class Singleton;
public:
	enum class EProtocol : ProtocolSize_t
	{
		None,

		CreateParty,			// 파티 생성		 , Result 있음
		RequestParticipate,		// 파티 참가 요청 , Result 있음

		NewParticipant,			// 새로운 파티 참가자, 기존 파티원에게 보낼때만

		Exit,					// 파티에서 나감
		Kick,					// 파티에서 강퇴됨
		TransferOwner,			// 파티장 위임

		AllPartyInfo,			// 모든 파티 정보
	};

	enum class EResult : ResultSize_t
	{
		None,

		PartyCreated,			// 파티 생성 완료

		RequestAccept,			// 파티 참가 신청 수락
		RequestDeny,			// 파티 참가 신청 거부
	};

private:
	uint32_t m_newPartyID;
	std::unordered_map<uint32_t, PlayerPartyInfoPtr> m_party_map;
	MyBase::CriticalSection m_cs;

	PlayerPartyManager() :m_newPartyID(1), m_party_map() {}

public:
	// 새로운 파티를 생성
	void CreateParty(NetBase::OutputMemoryStreamPtr& outpStreams, PlayerInfoPtr inpPlayer,
		std::wstring inParty_name, int inMax_capacity)
	{
		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 새로운 방 생성 및 초기화
		int party_id = m_newPartyID++;
		auto newParty = PlayerPartyInfo::Create(party_id, inpPlayer, inParty_name, inMax_capacity);
		m_party_map.insert({ party_id,newParty });

		// lock free
		m_cs.UnLock();

		// 방생성이 완료됬음을 클라이언트에게 전송
		// Protocol(CreateParty) + Result(PartyCreated) + PartyInfo
		int write_size = 0;
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)PlayerPartyManager::EProtocol::RequestParticipate);
		write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::PartyCreated);
		write_size += NetBase::WriteToBinStream(stream, newParty);

		// out stream 셋팅
		outpStreams = stream;
	}

	// 해당 id의 파티를 제거
	void DestroyParty(uint32_t inParty_id)
	{
		// 관리 리스트에 존재하는 경우 삭제
		if (m_party_map.find(inParty_id) != m_party_map.end())
			m_party_map.erase(inParty_id);
	}

	// 생성되어있는 모든 파티의 정보를 stream에 쓰기
	void SendALLPartyList(std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamVector)
	{	// m_party_map 의 정보를 모두 보내기

		// lock
		MyBase::AutoLocker lock(&m_cs);
		std::vector<PlayerPartyInfoPtr> m_party_vec(m_party_map.size());
		for (auto& item : m_party_map)
		{
			m_party_vec.push_back(item.second);
		}
		// lock free
		lock.ManualUnlock();

		// 한번에 최대 10개의 파티 정보를 보내기
		size_t total_count = m_party_vec.size();
		int cur_index = 0;
		while (total_count)
		{
			size_t cur_count;

			// 이번 사이클에 보낼 양 결정
			(total_count > 10) ? (cur_count = 10) : (cur_count = total_count);
			total_count -= cur_count;

			// send stream 가져오기
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			int write_size = 0;

			// data 를 stream에 쓰기
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::AllPartyInfo);
			write_size += NetBase::WriteToBinStream(stream, cur_count);
			for (size_t i = 0; i < cur_count; i++)
			{
				write_size += NetBase::WriteToBinStream(stream, m_party_vec[cur_index++]);
			}
#ifdef __DEBUG	
			printf("PartyManager SendALLPartyList write to stream : %dbytes\n", write_size);
#endif
			// out stream 셋팅
			outpStreamVector.push_back(stream);
		}
	}

	// 파티 신청 프로세스
	// 신청한 player의 characterinfo 를 party owner 에게 전송할 데이터를 처리
	void RequestParticipate(
		NetBase::OutputMemoryStreamPtr outpStreamToOwner
		, PlayerInfoPtr inVolunteer, uint32_t inParty_id)
	{
		outpStreamToOwner = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// is exist party ?
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// 해당 파티의 인원수를 확인
			if (iter->second->IsFull())
				return;

			// 지원자의 캐릭터 정보
			auto charInfo = inVolunteer->GetCharacterInfo();

			outpStreamToOwner = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// protocol + volunteer's character info
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ProtocolSize_t)EProtocol::RequestParticipate);
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, charInfo);
		}

		// lock free
		lock.ManualUnlock();
	}

	// 파티장이 파티 신청을 수락했을 때의 프로세스
	// 1. 신청한 Player에게 파티원들의 정보를 전송
	// 2. 신청한 PlayerInfo를 파티에 등록하고 ( => index )
	// 3. 모든 파티원에게 새로운 PlayerInfo 를 전송
	void AcceptPlayer(
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,		// 기존에 존재하던 파티원들의 session
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamToExistingPlayer,	// 기존에 존재하는 파티원들에게 보낼 stream
		IOCPSessionPtr& outpVolunteerSession,										// 신청한 player의 session
		NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer,						// 신청한 player에게 보낼 stream
		PlayerInfoPtr inVolunteer, uint32_t inParty_id)
	{
		outpExistingPlayerSessions.clear();
		outpStreamToExistingPlayer.clear();
		outpVolunteerSession = nullptr;
		outpStreamToVolunteer = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 해당 파티를 검색
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// 파티가 존재 하면
			auto party_info = iter->second;
			// 파티가 꽉 차있는 경우 함수 종료
			if (party_info->IsFull())
				return;

			// 기존 파티원들의 owner index , index vec , player vec
			int owner_index = party_info->m_owner_index;
			std::vector<int> m_existingPlayerIndexes;		// index vector
			std::vector <PlayerInfoPtr> m_existingPlayers;	// info vector
			for (int i = 0; party_info->m_player_vec.size(); i++)
			{
				if (party_info->m_player_vec[i] != nullptr)
				{
					m_existingPlayerIndexes.push_back(i);
					m_existingPlayers.push_back(party_info->m_player_vec[i]);
				}
			}

			{	//// 참가자에게 보낼 stream 작성
				// protocol + result + party owner index + index1 + playerinfo 1 + index2 + playerinfo2 ...
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::RequestParticipate);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::RequestAccept);
				NetBase::WriteToBinStream(stream, party_info->m_owner_index);
				for (int i = 0; i < m_existingPlayers.size(); i++)
				{
					NetBase::WriteToBinStream(stream, m_existingPlayerIndexes[i]);	// index
					NetBase::WriteToBinStream(stream, m_existingPlayers[i]);		// player info
				}
				outpVolunteerSession = inVolunteer->GetSession();
				outpStreamToVolunteer = stream;
			}	//// 참가자에게 보낼 stream 작성 end


			// 파티에 참가시켜 파티정보 최신화
			// 참가자의 위치 index
			int volunteer_index = party_info->Participate(inVolunteer);

			{	//// 기존 파티원들에게 보낼 stream 작성
				for (auto& item : party_info->m_player_vec)
				{	// 해당 index에 파티원이 존재하는지 확인
					if (item != nullptr)
					{
						// stream 작성
						// protocol + volunteer index + volunteer player info
						auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
						NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::NewParticipant);
						NetBase::WriteToBinStream(stream, volunteer_index);
						NetBase::WriteToBinStream(stream, inVolunteer);

						// out param 셋팅
						outpExistingPlayerSessions.push_back(item->GetSession());
						outpStreamToExistingPlayer.push_back(stream);
					}
				}
			}	//// 기존 파티원들에게 보낼 stream 작성 end
		}

		// lock free
		lock.ManualUnlock();
	}

	// 파티장이 파티를 거부했을 때의 프로세스
	void DenyPlayer()
	{

	}

	void ParticipateParty(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamToPartyPlayers,
		NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer,
		PlayerInfoPtr inpVolunteer, uint32_t inParty_id)
	{
		// lock
		MyBase::AutoLocker lock(&m_cs);

		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist		
			{
				// 참가 신청한 player 에게 참가가 수락되었음을 알림
				outpStreamToVolunteer = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				int write_size = 0;
				write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ProtocolSize_t)EProtocol::RequestParticipate);
				write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ResultSize_t)EResult::RequestAccept);
			}

			{
				outpStreamToPartyPlayers.clear();
				// 신청한 player 의 캐릭터 정보
				auto char_info = inpVolunteer->GetCharacterInfo();

				// 파티원을 순회하면서 stream 쓰기
				for (auto& item : iter->second->m_player_vec)
				{
					auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

					// 기존 파티원들에게 새로 참가하는 파티원의 정보를 전송
					int write_size = 0;
					write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::NewParticipant);
					write_size += NetBase::WriteToBinStream(stream, char_info);

					outpStreamToPartyPlayers.push_back(stream);
				}
			}

			// party 정보 갱신
			// 파티 신청자를 파티에 등록
			iter->second->Participate(inpVolunteer);
		}

		// lock free
		lock.ManualUnlock();
	}


};

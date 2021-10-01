#pragma once

class PlayerPartyManager : public MyBase::Singleton<PlayerPartyManager>
{
	friend class Singleton;
public:
	// 30 번 부터 ~ 39 번 까지 사용
	enum class EProtocol : ProtocolSize_t
	{
		None = 0,

		CreateParty = 1ULL << 30,			// 파티 생성		 , Result 있음
		RequestParticipate = 1ULL << 31,	// 파티 참가 요청 , Result 있음

		NewParticipant = 1ULL << 32,		// 새로운 파티 참가자, 기존 파티원에게 보낼때만

		Exit = 1ULL << 33,					// 자신이 파티에서 나감
		Kick = 1ULL << 34,					// 파티장이 파티에서 강퇴
		TransferOwner = 1ULL << 35,			// 파티장 위임

		AllPartyInfo = 1ULL << 36,			// 모든 파티 정보
	};

	enum class EResult : ResultSize_t
	{
		None = 0,

		PartyCreated = 1UL << 0,			// 파티 생성 완료

		RequestChecking = 1UL << 1,		// 파티 참가 신청 완료,
		RequestAccept = 1UL << 2,			// 파티 참가 신청을 파티장이 수락
		RequestReject = 1UL << 3,			// 파티 참가 신청을 파티장이 거부

		ExitComplete = 1UL << 4,			// 파티 퇴장 완료
		ExitCompleteOther = 1UL << 5,		// 파티원 중 한명이 파티를 퇴장

		Kicked = 1U << 6,				// 강퇴 당함
		KickedOther = 1U << 7,			// 파티원 중 한명이 강퇴당함				

		NotExistParty = 1U << 8,			// 존재하지 않는 파티
	};
public:
	// class 초기화
	virtual bool Initialize(LPVOID) noexcept;
	// class 정리
	virtual void Finalize() noexcept;
private:
	uint32_t m_newPartyID;
	std::unordered_map<uint32_t, PlayerPartyInfoPtr> m_party_map;
	MyBase::CriticalSection m_cs;

	PlayerPartyManager() :m_newPartyID(1), m_party_map() {}
	// inProtocol 을 inoutProtocol 에 추가
	void AddProtocol(EProtocol& inoutProtocol, const EProtocol& inProtocol) {
		inoutProtocol = (EProtocol)((ProtocolSize_t)inProtocol | (ProtocolSize_t)inoutProtocol);
	}
	// inResult 를 inoutResult 에 추가
	void AddResult(EResult& inoutResult, const EResult& inResult) {
		inoutResult = (EResult)((ResultSize_t)inResult | (ResultSize_t)inoutResult);
	}
public:
	// 새로운 파티를 생성
	void CreateParty(NetBase::OutputMemoryStreamPtr& outpStreams, NetBase::InputMemoryStreamPtr inpStream, PlayerInfoPtr inpPlayer)
	{
		// stream 분석
		// party name + max player count
		std::wstring party_name;
		int max_capacity;
		NetBase::ReadFromBinStream(inpStream, party_name);
		NetBase::ReadFromBinStream(inpStream, max_capacity);

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 새로운 방 생성 및 초기화
		int party_id = m_newPartyID++;
		auto newParty = PlayerPartyInfo::Create(party_id, inpPlayer, party_name, max_capacity);
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
			// protocol + vector size + party player vector content
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

			outpStreamToOwner = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// protocol + result + volunteer's player info
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ProtocolSize_t)EProtocol::RequestParticipate);
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ResultSize_t)EResult::RequestChecking);
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, inVolunteer);
		}
		else
		{	// 이미 사라진 파티인 경우
			outpStreamToOwner = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// protocol + result 
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ProtocolSize_t)EProtocol::RequestParticipate);
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ResultSize_t)EResult::NotExistParty);
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
		IOCP_Base::IOCPSessionBasePtr& outpVolunteerSession,										// 신청한 player의 session
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

			{	//// 참가자에게 보낼 stream 작성
				// protocol + result + party owner index + index1 + playerinfo 1 + index2 + playerinfo2 ...
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::RequestParticipate);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::RequestAccept);
				NetBase::WriteToBinStream(stream, party_info);

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
	void RejectPlayer(NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer, PlayerInfoPtr inVolunteer)
	{
		outpStreamToVolunteer = nullptr;

		// protocol + result 
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::RequestParticipate);
		NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::RequestReject);

		outpStreamToVolunteer = stream;
	}

	// 파티에서 자신이 직접 나갈 경우
	void ExitPlayer(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamExistingPlayers,		// 남아있는 파티원들에게 보낼 stream
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,		// 위에 대응하는 session pointer
		NetBase::OutputMemoryStreamPtr& outpStreamToExitPlayer,		// 파티에서 퇴장한 player에게 보낼 stream
		IOCP_Base::IOCPSessionBasePtr& outpExitPlayerSession,		// 위에 대응하는 session pointer
		PlayerInfoPtr inpInfo, uint32_t inParty_id)
	{
		outpStreamExistingPlayers.clear();
		outpExistingPlayerSessions.clear();
		outpStreamToExitPlayer = nullptr;
		outpExitPlayerSession = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 파티가 존재하는 지 체크
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// 파티에서 퇴장 처리
			// 새로운 파티장(newPartyOwner)
			auto newPartyOwner = iter->second->Exit(inpInfo);

			{	// 퇴장 완료 stream 작성
				// protocol + result
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::Exit);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::ExitComplete);

				outpStreamToExitPlayer = stream;
				outpExitPlayerSession = inpInfo->GetSession();
			}

			{	// 남아있는 파티원들에게 퇴장을 알림

				// protocol 작성		
				// result 작성
				EProtocol protocol;
				EResult result;
				if (newPartyOwner != nullptr)
				{	// 파티장이 바뀐 경우
					AddProtocol(protocol, EProtocol::TransferOwner);
					AddResult(result, EResult::None);
				}
				AddProtocol(protocol, EProtocol::Exit);
				AddResult(result, EResult::ExitCompleteOther);

				// stream 작성	
				// protocol + result + 퇴장하는 playerinfo + (새로운 파티장 playerinfo)
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
				NetBase::WriteToBinStream(stream, (ResultSize_t)result);
				NetBase::WriteToBinStream(stream, inpInfo);	// 퇴장하는 player info
				if (newPartyOwner != nullptr)
				{	// 파티장이 바뀐 경우
					NetBase::WriteToBinStream(stream, newPartyOwner);	// 새로운 파티장 player info
				}

				// 파티원들에게 보낼 data stream 에 복사
				for (auto& item : iter->second->m_player_vec) {
					if (item != nullptr)
					{
						auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
						*sendstream = *stream;	// copy

						outpStreamExistingPlayers.push_back(stream);
						outpExistingPlayerSessions.push_back(item->GetSession());
					}
				}
				// send용 이 아닌 stream 회수
				NetBase::PacketManager::sInstance->RetrieveSendStream(stream);
			}
		}
		else
		{	// 파티가 존재하지 않는 경우
			// 고려 안함
		}

		// lock free
		lock.ManualUnlock();
	}

	// 파티장이 파티원 중 한명을 Kick한 경우
	void KickPlayer(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamExistingPlayers,		// 남아있는 파티원들에게 보낼 stream
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,	// 위에 대응하는 session pointer
		NetBase::OutputMemoryStreamPtr& outpStreamToKickedPlayer,	// 파티에서 kick 당한 player에게 보낼 stream
		IOCP_Base::IOCPSessionBasePtr& outpKickedPlayerSession,		// 위에 대응하는 session pointer
		PlayerInfoPtr inpPlayerInfo, uint32_t inParty_id)
	{
		outpStreamExistingPlayers.clear();
		outpExistingPlayerSessions.clear();
		outpStreamToKickedPlayer = nullptr;
		outpKickedPlayerSession = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 파티가 존재하는지 체크
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// 파티에서 kick 처리
			iter->second->Kick(inpPlayerInfo);

			{	// 강퇴당한 player stream 작성
				// protocol + result
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::Kick);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::Kicked);

				outpStreamToKickedPlayer = stream;
				outpKickedPlayerSession = inpPlayerInfo->GetSession();
			}

			{	// 남아있는 파티원들에게 보낼 stream 작성				
				for (auto& item : iter->second->m_player_vec) {
					// protocol + result + kick 당한 playerinfo
					auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
					NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::Kick);
					NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::KickedOther);
					NetBase::WriteToBinStream(stream, inpPlayerInfo);

					outpStreamExistingPlayers.push_back(stream);
					outpExistingPlayerSessions.push_back(item->GetSession());
				}
			}
		}
		// lock free
		lock.ManualUnlock();
	}



	// 파티장이 새로운 파티장을 임명하는 경우
	void TransferPartyOwner(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamExistingPlayers,		// 파티원들에게 보낼 stream
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,		// 위에 대응하는 session pointer
		PlayerInfoPtr inpNewOwnerPlayer, uint32_t inParty_id)
	{
		outpStreamExistingPlayers.clear();
		outpExistingPlayerSessions.clear();

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 해당 파티가 존재하는지 체크
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// 파티 정보를 갱신
			iter->second->TransferPartyOwner(inpNewOwnerPlayer);

			// 모든 파티원들에게 보낼 stream 작성
			for (auto& item : iter->second->m_player_vec) {
				// protocol + result (none) + 새로운 파티장 palyerinfo 
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::TransferOwner);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::None);
				NetBase::WriteToBinStream(stream, inpNewOwnerPlayer);

				outpStreamExistingPlayers.push_back(stream);
				outpExistingPlayerSessions.push_back(item->GetSession());
			}
		}
		// unlock
		lock.ManualUnlock();
	}














	// 파티 참가
	//void ParticipateParty(
	//	std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamToPartyPlayers,
	//	NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer,
	//	PlayerInfoPtr inpVolunteer, uint32_t inParty_id)
	//{
	//	// lock
	//	MyBase::AutoLocker lock(&m_cs);

	//	auto iter = m_party_map.find(inParty_id);
	//	if (iter != m_party_map.end())
	//	{	// exist		
	//		{
	//			// 참가 신청한 player 에게 참가가 수락되었음을 알림
	//			outpStreamToVolunteer = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
	//			int write_size = 0;
	//			write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ProtocolSize_t)EProtocol::RequestParticipate);
	//			write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ResultSize_t)EResult::RequestAccept);
	//		}

	//		{
	//			outpStreamToPartyPlayers.clear();
	//			// 신청한 player 의 캐릭터 정보
	//			auto char_info = inpVolunteer->GetCharacterInfo();

	//			// 파티원을 순회하면서 stream 쓰기
	//			for (auto& item : iter->second->m_player_vec)
	//			{
	//				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

	//				// 기존 파티원들에게 새로 참가하는 파티원의 정보를 전송					
	//				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::NewParticipant);
	//				NetBase::WriteToBinStream(stream, char_info);

	//				outpStreamToPartyPlayers.push_back(stream);
	//			}
	//		}
	//		// party 정보 갱신
	//		// 파티 신청자를 파티에 등록
	//		iter->second->Participate(inpVolunteer);
	//	}
	//	// lock free
	//	lock.ManualUnlock();
	//}
};

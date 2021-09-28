#pragma once

class PlayerPartyManager : MyBase::Singleton<PlayerPartyManager>
{
	friend class Singleton;
public:
	enum class EProtocol : ProtocolSize_t
	{
		None,

		CreateParty,			// 파티 생성
		RequestParticipate,		// 파티 참가 요청
		Participate,			// 파티에 참가됨
		Exit,					// 파티에서 나감
		Kick,					// 파티에서 강퇴됨
		TransferOwner,			// 파티장 위임

		AllPartyInfo,			// 모든 파티 정보
	};

	enum class EResult : ResultSize_t
	{
		None,

		PartyCreated,			// 파티 생성 완료
	};

private:
	uint32_t m_newPartyID;
	std::unordered_map<uint32_t, PlayerPartyInfoPtr> m_party_map;
	MyBase::CriticalSection m_cs;

	PlayerPartyManager() :m_newPartyID(1), m_party_map() {}

public:
	void CreateParty(PlayerInfoPtr creater, std::wstring inParty_name, int inMax_capacity)
	{
		// lock
		MyBase::AutoLocker lock(&m_cs);

		// 새로운 방 생성 및 초기화
		int party_id = m_newPartyID++;
		auto newParty = PlayerPartyInfo::Create(party_id, creater, inParty_name, inMax_capacity);
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

		// client 에게 전송
		creater->GetSession()->Send(stream);
	}
	void DestroyParty(uint32_t inParty_id)
	{
		// 관리 리스트에 존재하는 경우 삭제
		if (m_party_map.find(inParty_id) != m_party_map.end())
			m_party_map.erase(inParty_id);
	}

	void SendALLPartyList(IOCPSessionPtr inpSession)
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
			// 클라이언트에게 전송
			inpSession->Send(stream);
		}
	}

	void RequestParticipate(PlayerInfoPtr inpInfo, uint32_t inParty_id)
	{

	}

	void ParticipateParty(PlayerInfoPtr inpInfo, uint32_t inParty_id)
	{

	}
};

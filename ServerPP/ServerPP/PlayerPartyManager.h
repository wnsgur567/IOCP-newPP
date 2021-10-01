#pragma once

class PlayerPartyManager : public MyBase::Singleton<PlayerPartyManager>
{
	friend class Singleton;
public:
	// 30 �� ���� ~ 39 �� ���� ���
	enum class EProtocol : ProtocolSize_t
	{
		None = 0,

		CreateParty = 1ULL << 30,			// ��Ƽ ����		 , Result ����
		RequestParticipate = 1ULL << 31,	// ��Ƽ ���� ��û , Result ����

		NewParticipant = 1ULL << 32,		// ���ο� ��Ƽ ������, ���� ��Ƽ������ ��������

		Exit = 1ULL << 33,					// �ڽ��� ��Ƽ���� ����
		Kick = 1ULL << 34,					// ��Ƽ���� ��Ƽ���� ����
		TransferOwner = 1ULL << 35,			// ��Ƽ�� ����

		AllPartyInfo = 1ULL << 36,			// ��� ��Ƽ ����
	};

	enum class EResult : ResultSize_t
	{
		None = 0,

		PartyCreated = 1UL << 0,			// ��Ƽ ���� �Ϸ�

		RequestChecking = 1UL << 1,		// ��Ƽ ���� ��û �Ϸ�,
		RequestAccept = 1UL << 2,			// ��Ƽ ���� ��û�� ��Ƽ���� ����
		RequestReject = 1UL << 3,			// ��Ƽ ���� ��û�� ��Ƽ���� �ź�

		ExitComplete = 1UL << 4,			// ��Ƽ ���� �Ϸ�
		ExitCompleteOther = 1UL << 5,		// ��Ƽ�� �� �Ѹ��� ��Ƽ�� ����

		Kicked = 1U << 6,				// ���� ����
		KickedOther = 1U << 7,			// ��Ƽ�� �� �Ѹ��� �������				

		NotExistParty = 1U << 8,			// �������� �ʴ� ��Ƽ
	};
public:
	// class �ʱ�ȭ
	virtual bool Initialize(LPVOID) noexcept;
	// class ����
	virtual void Finalize() noexcept;
private:
	uint32_t m_newPartyID;
	std::unordered_map<uint32_t, PlayerPartyInfoPtr> m_party_map;
	MyBase::CriticalSection m_cs;

	PlayerPartyManager() :m_newPartyID(1), m_party_map() {}
	// inProtocol �� inoutProtocol �� �߰�
	void AddProtocol(EProtocol& inoutProtocol, const EProtocol& inProtocol) {
		inoutProtocol = (EProtocol)((ProtocolSize_t)inProtocol | (ProtocolSize_t)inoutProtocol);
	}
	// inResult �� inoutResult �� �߰�
	void AddResult(EResult& inoutResult, const EResult& inResult) {
		inoutResult = (EResult)((ResultSize_t)inResult | (ResultSize_t)inoutResult);
	}
public:
	// ���ο� ��Ƽ�� ����
	void CreateParty(NetBase::OutputMemoryStreamPtr& outpStreams, NetBase::InputMemoryStreamPtr inpStream, PlayerInfoPtr inpPlayer)
	{
		// stream �м�
		// party name + max player count
		std::wstring party_name;
		int max_capacity;
		NetBase::ReadFromBinStream(inpStream, party_name);
		NetBase::ReadFromBinStream(inpStream, max_capacity);

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// ���ο� �� ���� �� �ʱ�ȭ
		int party_id = m_newPartyID++;
		auto newParty = PlayerPartyInfo::Create(party_id, inpPlayer, party_name, max_capacity);
		m_party_map.insert({ party_id,newParty });

		// lock free
		m_cs.UnLock();

		// ������� �Ϸ������ Ŭ���̾�Ʈ���� ����
		// Protocol(CreateParty) + Result(PartyCreated) + PartyInfo
		int write_size = 0;
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)PlayerPartyManager::EProtocol::RequestParticipate);
		write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::PartyCreated);
		write_size += NetBase::WriteToBinStream(stream, newParty);

		// out stream ����
		outpStreams = stream;
	}

	// �ش� id�� ��Ƽ�� ����
	void DestroyParty(uint32_t inParty_id)
	{
		// ���� ����Ʈ�� �����ϴ� ��� ����
		if (m_party_map.find(inParty_id) != m_party_map.end())
			m_party_map.erase(inParty_id);
	}

	// �����Ǿ��ִ� ��� ��Ƽ�� ������ stream�� ����
	void SendALLPartyList(std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamVector)
	{	// m_party_map �� ������ ��� ������

		// lock
		MyBase::AutoLocker lock(&m_cs);
		std::vector<PlayerPartyInfoPtr> m_party_vec(m_party_map.size());
		for (auto& item : m_party_map)
		{
			m_party_vec.push_back(item.second);
		}
		// lock free
		lock.ManualUnlock();

		// �ѹ��� �ִ� 10���� ��Ƽ ������ ������
		size_t total_count = m_party_vec.size();
		int cur_index = 0;
		while (total_count)
		{
			size_t cur_count;

			// �̹� ����Ŭ�� ���� �� ����
			(total_count > 10) ? (cur_count = 10) : (cur_count = total_count);
			total_count -= cur_count;

			// send stream ��������
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			int write_size = 0;

			// data �� stream�� ����
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
			// out stream ����
			outpStreamVector.push_back(stream);
		}
	}

	// ��Ƽ ��û ���μ���
	// ��û�� player�� characterinfo �� party owner ���� ������ �����͸� ó��
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
			// �ش� ��Ƽ�� �ο����� Ȯ��
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
		{	// �̹� ����� ��Ƽ�� ���
			outpStreamToOwner = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// protocol + result 
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ProtocolSize_t)EProtocol::RequestParticipate);
			write_size += NetBase::WriteToBinStream(outpStreamToOwner, (ResultSize_t)EResult::NotExistParty);
		}

		// lock free
		lock.ManualUnlock();
	}

	// ��Ƽ���� ��Ƽ ��û�� �������� ���� ���μ���
	// 1. ��û�� Player���� ��Ƽ������ ������ ����
	// 2. ��û�� PlayerInfo�� ��Ƽ�� ����ϰ� ( => index )
	// 3. ��� ��Ƽ������ ���ο� PlayerInfo �� ����
	void AcceptPlayer(
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,		// ������ �����ϴ� ��Ƽ������ session
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamToExistingPlayer,	// ������ �����ϴ� ��Ƽ���鿡�� ���� stream
		IOCP_Base::IOCPSessionBasePtr& outpVolunteerSession,										// ��û�� player�� session
		NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer,						// ��û�� player���� ���� stream
		PlayerInfoPtr inVolunteer, uint32_t inParty_id)
	{
		outpExistingPlayerSessions.clear();
		outpStreamToExistingPlayer.clear();
		outpVolunteerSession = nullptr;
		outpStreamToVolunteer = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// �ش� ��Ƽ�� �˻�
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// ��Ƽ�� ���� �ϸ�
			auto party_info = iter->second;
			// ��Ƽ�� �� ���ִ� ��� �Լ� ����
			if (party_info->IsFull())
				return;

			{	//// �����ڿ��� ���� stream �ۼ�
				// protocol + result + party owner index + index1 + playerinfo 1 + index2 + playerinfo2 ...
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::RequestParticipate);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::RequestAccept);
				NetBase::WriteToBinStream(stream, party_info);

				outpVolunteerSession = inVolunteer->GetSession();
				outpStreamToVolunteer = stream;
			}	//// �����ڿ��� ���� stream �ۼ� end


			// ��Ƽ�� �������� ��Ƽ���� �ֽ�ȭ
			// �������� ��ġ index
			int volunteer_index = party_info->Participate(inVolunteer);

			{	//// ���� ��Ƽ���鿡�� ���� stream �ۼ�
				for (auto& item : party_info->m_player_vec)
				{	// �ش� index�� ��Ƽ���� �����ϴ��� Ȯ��
					if (item != nullptr)
					{
						// stream �ۼ�
						// protocol + volunteer index + volunteer player info
						auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
						NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::NewParticipant);
						NetBase::WriteToBinStream(stream, volunteer_index);
						NetBase::WriteToBinStream(stream, inVolunteer);

						// out param ����
						outpExistingPlayerSessions.push_back(item->GetSession());
						outpStreamToExistingPlayer.push_back(stream);
					}
				}
			}	//// ���� ��Ƽ���鿡�� ���� stream �ۼ� end
		}

		// lock free
		lock.ManualUnlock();
	}

	// ��Ƽ���� ��Ƽ�� �ź����� ���� ���μ���
	void RejectPlayer(NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer, PlayerInfoPtr inVolunteer)
	{
		outpStreamToVolunteer = nullptr;

		// protocol + result 
		auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
		NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::RequestParticipate);
		NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::RequestReject);

		outpStreamToVolunteer = stream;
	}

	// ��Ƽ���� �ڽ��� ���� ���� ���
	void ExitPlayer(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamExistingPlayers,		// �����ִ� ��Ƽ���鿡�� ���� stream
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,		// ���� �����ϴ� session pointer
		NetBase::OutputMemoryStreamPtr& outpStreamToExitPlayer,		// ��Ƽ���� ������ player���� ���� stream
		IOCP_Base::IOCPSessionBasePtr& outpExitPlayerSession,		// ���� �����ϴ� session pointer
		PlayerInfoPtr inpInfo, uint32_t inParty_id)
	{
		outpStreamExistingPlayers.clear();
		outpExistingPlayerSessions.clear();
		outpStreamToExitPlayer = nullptr;
		outpExitPlayerSession = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// ��Ƽ�� �����ϴ� �� üũ
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// ��Ƽ���� ���� ó��
			// ���ο� ��Ƽ��(newPartyOwner)
			auto newPartyOwner = iter->second->Exit(inpInfo);

			{	// ���� �Ϸ� stream �ۼ�
				// protocol + result
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::Exit);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::ExitComplete);

				outpStreamToExitPlayer = stream;
				outpExitPlayerSession = inpInfo->GetSession();
			}

			{	// �����ִ� ��Ƽ���鿡�� ������ �˸�

				// protocol �ۼ�		
				// result �ۼ�
				EProtocol protocol;
				EResult result;
				if (newPartyOwner != nullptr)
				{	// ��Ƽ���� �ٲ� ���
					AddProtocol(protocol, EProtocol::TransferOwner);
					AddResult(result, EResult::None);
				}
				AddProtocol(protocol, EProtocol::Exit);
				AddResult(result, EResult::ExitCompleteOther);

				// stream �ۼ�	
				// protocol + result + �����ϴ� playerinfo + (���ο� ��Ƽ�� playerinfo)
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)protocol);
				NetBase::WriteToBinStream(stream, (ResultSize_t)result);
				NetBase::WriteToBinStream(stream, inpInfo);	// �����ϴ� player info
				if (newPartyOwner != nullptr)
				{	// ��Ƽ���� �ٲ� ���
					NetBase::WriteToBinStream(stream, newPartyOwner);	// ���ο� ��Ƽ�� player info
				}

				// ��Ƽ���鿡�� ���� data stream �� ����
				for (auto& item : iter->second->m_player_vec) {
					if (item != nullptr)
					{
						auto sendstream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
						*sendstream = *stream;	// copy

						outpStreamExistingPlayers.push_back(stream);
						outpExistingPlayerSessions.push_back(item->GetSession());
					}
				}
				// send�� �� �ƴ� stream ȸ��
				NetBase::PacketManager::sInstance->RetrieveSendStream(stream);
			}
		}
		else
		{	// ��Ƽ�� �������� �ʴ� ���
			// ��� ����
		}

		// lock free
		lock.ManualUnlock();
	}

	// ��Ƽ���� ��Ƽ�� �� �Ѹ��� Kick�� ���
	void KickPlayer(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamExistingPlayers,		// �����ִ� ��Ƽ���鿡�� ���� stream
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,	// ���� �����ϴ� session pointer
		NetBase::OutputMemoryStreamPtr& outpStreamToKickedPlayer,	// ��Ƽ���� kick ���� player���� ���� stream
		IOCP_Base::IOCPSessionBasePtr& outpKickedPlayerSession,		// ���� �����ϴ� session pointer
		PlayerInfoPtr inpPlayerInfo, uint32_t inParty_id)
	{
		outpStreamExistingPlayers.clear();
		outpExistingPlayerSessions.clear();
		outpStreamToKickedPlayer = nullptr;
		outpKickedPlayerSession = nullptr;

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// ��Ƽ�� �����ϴ��� üũ
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// ��Ƽ���� kick ó��
			iter->second->Kick(inpPlayerInfo);

			{	// ������� player stream �ۼ�
				// protocol + result
				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::Kick);
				NetBase::WriteToBinStream(stream, (ResultSize_t)EResult::Kicked);

				outpStreamToKickedPlayer = stream;
				outpKickedPlayerSession = inpPlayerInfo->GetSession();
			}

			{	// �����ִ� ��Ƽ���鿡�� ���� stream �ۼ�				
				for (auto& item : iter->second->m_player_vec) {
					// protocol + result + kick ���� playerinfo
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



	// ��Ƽ���� ���ο� ��Ƽ���� �Ӹ��ϴ� ���
	void TransferPartyOwner(
		std::vector<NetBase::OutputMemoryStreamPtr>& outpStreamExistingPlayers,		// ��Ƽ���鿡�� ���� stream
		std::vector<IOCP_Base::IOCPSessionBasePtr>& outpExistingPlayerSessions,		// ���� �����ϴ� session pointer
		PlayerInfoPtr inpNewOwnerPlayer, uint32_t inParty_id)
	{
		outpStreamExistingPlayers.clear();
		outpExistingPlayerSessions.clear();

		// lock
		MyBase::AutoLocker lock(&m_cs);

		// �ش� ��Ƽ�� �����ϴ��� üũ
		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist
			// ��Ƽ ������ ����
			iter->second->TransferPartyOwner(inpNewOwnerPlayer);

			// ��� ��Ƽ���鿡�� ���� stream �ۼ�
			for (auto& item : iter->second->m_player_vec) {
				// protocol + result (none) + ���ο� ��Ƽ�� palyerinfo 
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














	// ��Ƽ ����
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
	//			// ���� ��û�� player ���� ������ �����Ǿ����� �˸�
	//			outpStreamToVolunteer = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
	//			int write_size = 0;
	//			write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ProtocolSize_t)EProtocol::RequestParticipate);
	//			write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ResultSize_t)EResult::RequestAccept);
	//		}

	//		{
	//			outpStreamToPartyPlayers.clear();
	//			// ��û�� player �� ĳ���� ����
	//			auto char_info = inpVolunteer->GetCharacterInfo();

	//			// ��Ƽ���� ��ȸ�ϸ鼭 stream ����
	//			for (auto& item : iter->second->m_player_vec)
	//			{
	//				auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

	//				// ���� ��Ƽ���鿡�� ���� �����ϴ� ��Ƽ���� ������ ����					
	//				NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::NewParticipant);
	//				NetBase::WriteToBinStream(stream, char_info);

	//				outpStreamToPartyPlayers.push_back(stream);
	//			}
	//		}
	//		// party ���� ����
	//		// ��Ƽ ��û�ڸ� ��Ƽ�� ���
	//		iter->second->Participate(inpVolunteer);
	//	}
	//	// lock free
	//	lock.ManualUnlock();
	//}
};

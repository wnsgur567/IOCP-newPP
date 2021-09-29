#pragma once

class PlayerPartyManager : MyBase::Singleton<PlayerPartyManager>
{
	friend class Singleton;
public:
	enum class EProtocol : ProtocolSize_t
	{
		None,

		CreateParty,			// ��Ƽ ����
		RequestParticipate,		// ��Ƽ ���� ��û

		NewParticipant,			// ���ο� ��Ƽ ������

		Exit,					// ��Ƽ���� ����
		Kick,					// ��Ƽ���� �����
		TransferOwner,			// ��Ƽ�� ����

		AllPartyInfo,			// ��� ��Ƽ ����
	};

	enum class EResult : ResultSize_t
	{
		None,

		PartyCreated,			// ��Ƽ ���� �Ϸ�
		RequestAccept,			// ��Ƽ ���� ��û ����
		RequestDeny,			// ��Ƽ ���� ��û �ź�
	};

private:
	uint32_t m_newPartyID;
	std::unordered_map<uint32_t, PlayerPartyInfoPtr> m_party_map;
	MyBase::CriticalSection m_cs;

	PlayerPartyManager() :m_newPartyID(1), m_party_map() {}

public:
	void CreateParty(NetBase::OutputMemoryStreamPtr& outpStreams, PlayerInfoPtr inpPlayer, std::wstring inParty_name, int inMax_capacity)
	{
		// lock
		MyBase::AutoLocker lock(&m_cs);

		// ���ο� �� ���� �� �ʱ�ȭ
		int party_id = m_newPartyID++;
		auto newParty = PlayerPartyInfo::Create(party_id, inpPlayer, inParty_name, inMax_capacity);
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
	void DestroyParty(uint32_t inParty_id)
	{
		// ���� ����Ʈ�� �����ϴ� ��� ����
		if (m_party_map.find(inParty_id) != m_party_map.end())
			m_party_map.erase(inParty_id);
	}

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

	// send�� ���� session ���� outpstream�� ������ �� ���� �߻��� ���ɼ��� ��� �Ұ�
	void RequestParticipate(
		NetBase::OutputMemoryStreamPtr outpStream, IOCPSessionPtr outpPartyOwner
		, PlayerInfoPtr inVolunteer, uint32_t inParty_id)
	{
		// lock
		MyBase::AutoLocker lock(&m_cs);
		// is exist party ?
		if (m_party_map.find(inParty_id) != m_party_map.end())
		{	// exist
			// �������� ĳ���� ����
			auto charInfo = inVolunteer->GetCharacterInfo();

			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
			// protocol + volunteer's character info
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::RequestParticipate);
			write_size += NetBase::WriteToBinStream(stream, charInfo);
		}

		// lock free
		lock.ManualUnlock();
	}


	void ParticipateParty(
		std::vector< NetBase::OutputMemoryStreamPtr>& outpStreamToPartyPlayers,
		NetBase::OutputMemoryStreamPtr& outpStreamToVolunteer,
		PlayerInfoPtr inpVolunteer, uint32_t inParty_id)
	{
		// lock
		MyBase::AutoLocker lock(&m_cs);

		auto iter = m_party_map.find(inParty_id);
		if (iter != m_party_map.end())
		{	// exist		
			{
				// ���� ��û�� player ���� ������ �����Ǿ����� �˸�
				outpStreamToVolunteer = NetBase::PacketManager::sInstance->GetSendStreamFromPool();
				int write_size = 0;
				write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ProtocolSize_t)EProtocol::RequestParticipate);
				write_size += NetBase::WriteToBinStream(outpStreamToVolunteer, (ResultSize_t)EResult::RequestAccept);
			}

			{
				outpStreamToPartyPlayers.clear();
				// ��û�� player �� ĳ���� ����
				auto char_info = inpVolunteer->GetCharacterInfo();

				// ��Ƽ���� ��ȸ�ϸ鼭 stream ����
				for (auto& item : iter->second->m_player_vec)
				{
					auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

					// ���� ��Ƽ���鿡�� ���� �����ϴ� ��Ƽ���� ������ ����
					int write_size = 0;
					write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)EProtocol::NewParticipant);
					write_size += NetBase::WriteToBinStream(stream, char_info);

					outpStreamToPartyPlayers.push_back(stream);
				}
			}
			// party ���� ����
			iter->second->Participate(inpVolunteer);
		}

		// lock free
		lock.ManualUnlock();
	}
};

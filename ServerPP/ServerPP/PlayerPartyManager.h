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
		Participate,			// ��Ƽ�� ������
		Exit,					// ��Ƽ���� ����
		Kick,					// ��Ƽ���� �����
		TransferOwner,			// ��Ƽ�� ����

		AllPartyInfo,			// ��� ��Ƽ ����
	};

	enum class EResult : ResultSize_t
	{
		None,

		PartyCreated,			// ��Ƽ ���� �Ϸ�
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

		// ���ο� �� ���� �� �ʱ�ȭ
		int party_id = m_newPartyID++;
		auto newParty = PlayerPartyInfo::Create(party_id, creater, inParty_name, inMax_capacity);
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

		// client ���� ����
		creater->GetSession()->Send(stream);
	}
	void DestroyParty(uint32_t inParty_id)
	{
		// ���� ����Ʈ�� �����ϴ� ��� ����
		if (m_party_map.find(inParty_id) != m_party_map.end())
			m_party_map.erase(inParty_id);
	}

	void SendALLPartyList(IOCPSessionPtr inpSession)
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
			// Ŭ���̾�Ʈ���� ����
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

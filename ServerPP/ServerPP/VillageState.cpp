#include "IOCPNet_RootHeader.h"

void VillageState::OnRecvCompleted(NetBase::InputMemoryStreamPtr inpStream)
{
#ifdef __DEBUG
	printf("---VillageState OnRecvCompleted Process...\n");
#endif // __DEBUG
	auto owner = m_ownerPtr.lock();

	ProtocolSize_t raw_protocol;
	NetBase::ReadFromBinStream(inpStream, raw_protocol);
	//inpStream->Read(&raw_protocol, sizeof(ProtocolSize_t));

	// village protocol ó�� �б�
	EProtocol protocol;
	GetProtocol(raw_protocol, protocol);
	switch (protocol)
	{
	case EProtocol::EnterVillage:
	{
		Village::VillageManager::sInstance->VillageEnterProcess(inpStream, owner);
	}
	break;
	case EProtocol::ExitVillage:
	{
		Village::VillageManager::sInstance->VillageExitProcess(inpStream, owner);
	}
	break;
	}

	// sector protocol ó�� �б�
	ESectorProtocol sector_protocol;
	GetProtocol(raw_protocol, sector_protocol);
	switch (sector_protocol)
	{
	case ESectorProtocol::PlayerMoveAndAction:
	{
		Village::VillageManager::sInstance->VillageExitProcess(inpStream, owner);
	}
	break;
	case ESectorProtocol::PlayerAction:
	{
		Village::VillageManager::sInstance->VillageActionProcess(inpStream, owner);
	}
	break;
	case ESectorProtocol::PlayerMove:
	{
		Village::VillageManager::sInstance->VillageMoveProcess(inpStream, owner);
	}
	break;
	}

	// party protocol ó�� �б�
	EPartyProtocol party_protocol;
	GetProtocol(raw_protocol, party_protocol);
	switch (party_protocol)
	{
	case EPartyProtocol::CreateParty:
	{	// ��Ƽ ���� ���μ��� 
		NetBase::OutputMemoryStreamPtr outpSream;
		PlayerPartyManager::sInstance->CreateParty(outpSream, inpStream, owner->m_player);
		// ��Ƽ ���� ����� ����
		owner->Send(outpSream);
	}
	break;
	case EPartyProtocol::RequestParticipate:
	{
		// ��Ƽ���� session, stream
		NetBase::OutputMemoryStreamPtr outpStream;
		IOCP_Base::IOCPSessionBasePtr party_owner_session;
		// ���� ��û�� ����� �÷��̾� ����
		PlayerInfoPtr volunteer_info = owner->GetPlayerInfo();
		PlayerPartyManager::sInstance->RequestParticipate(outpStream, party_owner_session, inpStream, volunteer_info);

		// ��Ƽ�忡�� ��û �����͸� ����
		if (nullptr != party_owner_session)
		{
			party_owner_session->Send(outpStream);
		}
	}
	break;
	case EPartyProtocol::RequestReply:
	{
		using Result_t = PlayerPartyManager::EResult;
		uint32_t raw_result;
		NetBase::ReadFromBinStream(inpStream, raw_result);
		Result_t result = (Result_t)raw_result;

		// ���� ����
		if (result == Result_t::RequestAccept)
		{
			std::vector<IOCP_Base::IOCPSessionBasePtr> outpExistingPlayerSessions;
			std::vector<NetBase::OutputMemoryStreamPtr> outpStreamToExistingPlayer;
			IOCP_Base::IOCPSessionBasePtr outpVolunteerSession;
			NetBase::OutputMemoryStreamPtr outpStreamToVolunteer;

			auto party_info = owner->GetPartyInfo();

			PlayerPartyManager::sInstance->AcceptPlayer(
				outpExistingPlayerSessions,
				outpStreamToExistingPlayer,
				outpVolunteerSession,
				outpStreamToVolunteer,
				inpStream,
				party_info->GetID());

			for (size_t i = 0; i < outpExistingPlayerSessions.size(); i++)
			{
				outpExistingPlayerSessions[i]->Send(outpStreamToExistingPlayer[i]);
			}
			outpVolunteerSession->Send(outpStreamToVolunteer);
		}
		// ���� �ź�
		else if (result == Result_t::RequestReject)
		{
			IOCP_Base::IOCPSessionBasePtr outpVolunteerSession;
			NetBase::OutputMemoryStreamPtr outpStream;
			PlayerPartyManager::sInstance->RejectPlayer(outpStream, outpVolunteerSession, inpStream);

			outpVolunteerSession->Send(outpStream);
		}

		// send ó��
	}
	break;
	case EPartyProtocol::NewParticipant:
	{

	}
	break;
	case EPartyProtocol::Exit:
	{
		std::vector<NetBase::OutputMemoryStreamPtr> streamsToOtherPlayers;
		std::vector<IOCP_Base::IOCPSessionBasePtr> otherPlayersSession;
		NetBase::OutputMemoryStreamPtr streamToExitPlayer;
		IOCP_Base::IOCPSessionBasePtr exitPlayerSession;

		PlayerPartyManager::sInstance->ExitPlayer(
			streamsToOtherPlayers,
			otherPlayersSession,
			streamToExitPlayer,
			exitPlayerSession,
			inpStream,
			owner->m_player);

		for (size_t i = 0; i < streamsToOtherPlayers.size(); i++)
			otherPlayersSession[i]->Send(streamsToOtherPlayers[i]);
		exitPlayerSession->Send(streamToExitPlayer);
	}
	break;
	case EPartyProtocol::Kick:
	{

	}
	break;
	case EPartyProtocol::TransferOwner:
	{

	}
	break;
	case EPartyProtocol::AllPartyInfo:
	{	// ������ �����Ǿ��ִ� ��� ��Ƽ ������ ����
		std::vector<NetBase::OutputMemoryStreamPtr> outpStreamVec;
		PlayerPartyManager::sInstance->SendALLPartyList(outpStreamVec);
		// ��Ƽ ���� ����
		for (auto& item : outpStreamVec)
		{
			owner->Send(item);
		}
	}
	break;
	}
}

void VillageState::OnSendCompleted()
{
}

void VillageState::OnInitilzed()
{

}

void VillageState::OnChangedToThis()
{
#ifdef  __DEBUG
	printf("start Village State!!\n");
#endif //  __DEBUG

	// ���� ����� user �������� village id ���� �� ��		
	auto owner = m_ownerPtr.lock();
	m_current_result = Village::VillageManager::sInstance->StateChangedProcess(owner);
}

// Village protocol �����ϱ�
void VillageState::GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol)
{

	if ((ProtocolSize_t)EProtocol::EnterVillage & inOrigin)
	{
		outProtocol = EProtocol::EnterVillage;
		return;
	}
	if ((ProtocolSize_t)EProtocol::ExitVillage & inOrigin)
	{
		outProtocol = EProtocol::EnterVillage;
		return;
	}


}

// Sector �������� �����ϱ�
void VillageState::GetProtocol(ProtocolSize_t inOrigin, ESectorProtocol& outProtocol)
{
	if ((ProtocolSize_t)ESectorProtocol::PlayerMove & inOrigin)
	{
		if ((ProtocolSize_t)ESectorProtocol::PlayerAction & inOrigin)
		{
			outProtocol = ESectorProtocol::PlayerMoveAndAction;
			return;
		}
		outProtocol = ESectorProtocol::PlayerMove;
		return;
	}
	if ((ProtocolSize_t)ESectorProtocol::PlayerAction & inOrigin)
	{
		outProtocol = ESectorProtocol::PlayerAction;
		return;
	}
}

// Party �������� �����ϱ�
void VillageState::GetProtocol(ProtocolSize_t inOrigin, EPartyProtocol& outProtocol)
{
	if ((ProtocolSize_t)EPartyProtocol::AllPartyInfo & inOrigin)
	{
		outProtocol = EPartyProtocol::AllPartyInfo;
		return;
	}

	if ((ProtocolSize_t)EPartyProtocol::CreateParty & inOrigin)
	{
		outProtocol = EPartyProtocol::CreateParty;
		return;
	}
	if ((ProtocolSize_t)EPartyProtocol::Exit & inOrigin)
	{
		outProtocol = EPartyProtocol::Exit;
		return;
	}
	if ((ProtocolSize_t)EPartyProtocol::Kick & inOrigin)
	{
		outProtocol = EPartyProtocol::Kick;
		return;
	}
	if ((ProtocolSize_t)EPartyProtocol::NewParticipant & inOrigin)
	{
		outProtocol = EPartyProtocol::NewParticipant;
		return;
	}
	if ((ProtocolSize_t)EPartyProtocol::RequestParticipate & inOrigin)
	{
		outProtocol = EPartyProtocol::RequestParticipate;
		return;
	}
	if ((ProtocolSize_t)EPartyProtocol::RequestReply & inOrigin)
	{
		outProtocol = EPartyProtocol::RequestReply;
		return;
	}
	if ((ProtocolSize_t)EPartyProtocol::TransferOwner & inOrigin)
	{
		outProtocol = EPartyProtocol::TransferOwner;
		return;
	}
}




void VillageState::HandleAction(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream)
{

}

void VillageState::HandleMove(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&)
{


}

IOCPSession::ClientStatePtr VillageState::Create(IOCPSessionPtr pSession)
{
	std::shared_ptr<VillageState> retState;
	retState.reset(new VillageState(pSession));

	return retState;
}

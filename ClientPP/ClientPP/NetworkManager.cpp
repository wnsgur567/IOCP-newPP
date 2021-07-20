#include "base.h"

Implementation_sInstance(NetworkManagerClient);


// 임시
enum class EProtocol
{
	None,

	SignUp,
	SignIn,
	SignOut,
	SignResult,
};

bool NetworkManagerClient::DoFrame()
{
	SessionPtr pSession = std::static_pointer_cast<Session>(pSessionBase);

	ESessionState state = pSession->GetState();

	switch (state)
	{
	case ESessionState::None:
		break;
	case ESessionState::Conneted:
		break;
	case ESessionState::Sign:
	{
		printf("1.sign in\n2.sign up\n3.exit\n");
		int input;
		scanf("%d", &input);

		switch (input)
		{
		case 1:
		{
			char id[30];
			char pw[30];
			ZeroMemory(id, 30);
			ZeroMemory(pw, 30);
			printf("id : ");
			scanf("%s", id);
			printf("pw : ");
			scanf("%s", pw);

			SendPacketPtr pSendPacket = PacketManager::sInstance->GetSendPacketFromPool();
			OutputMemoryStreamPtr pStream = std::make_shared<OutputMemoryStream>(BUFSIZE);

			EProtocol protocol = EProtocol::SignIn;
			pStream->Write(&protocol, sizeof(EProtocol));
			size_t length = strlen(id);
			pStream->Write(&length, sizeof(size_t));
			pStream->Write(id, length);
			length = strlen(pw);
			pStream->Write(&length, sizeof(size_t));
			pStream->Write(pw, length);

			pSendPacket->SetStream(pStream);

			if (false == Send(pSendPacket))
			{
				// ...
			}

			RecvPacketPtr pRecvPacket;
			if (false == Recv(pRecvPacket))
			{
				// ...
			}


		}
		break;
		case 2:
		{
			char id[30];
			char pw[30];
			printf("id : ");
			scanf("%s", id);
			printf("pw : ");
			scanf("%s", pw);

			SendPacketPtr pSendPacket = PacketManager::sInstance->GetSendPacketFromPool();
			OutputMemoryStreamPtr pStream = std::make_shared<OutputMemoryStream>(BUFSIZE);

			EProtocol protocol = EProtocol::SignIn;
			pStream->Write(&protocol, sizeof(EProtocol));
			size_t length = strlen(id);
			pStream->Write(&length, sizeof(size_t));
			pStream->Write(id, length);
			length = strlen(pw);
			pStream->Write(&length, sizeof(size_t));
			pStream->Write(pw, length);

			pSendPacket->SetStream(pStream);

			if (false == Send(pSendPacket))
			{
				// ...
			}

			RecvPacketPtr pRecvPacket;
			if (false == Recv(pRecvPacket))
			{
				// ...
			}

		}
		break;
		case 3:
			break;
		}





	}
	break;
	case ESessionState::Disconneted:
		break;
	default:
		break;
	}

	return true;
}


bool NetworkManagerClient::Recv(RecvPacketPtr& outRecvPacket)
{
	TCPSocketPtr pSock = pSessionBase->GetSockPtr();

	auto pPacket = PacketManager::sInstance->GetRecvPacketFromPool();

	int size = pSock->Recv(&pPacket->m_target_recvbytes, sizeof(RecvPacket::psize_t));
	if (size == SOCKET_ERROR)
		return false;

	size = pSock->Recv(pPacket->m_buf, pPacket->m_target_recvbytes);
	if (size == SOCKET_ERROR)
		return false;

	// 중복 패킷인 경우
	if (pSessionBase->IsDuplicatedPacket(pPacket->GetId()))
		return Recv(outRecvPacket);

	// 정상 처리
	outRecvPacket = pPacket;

	return true;
}

bool NetworkManagerClient::Send(SendPacketPtr inpPacket)
{
	TCPSocketPtr pSock = pSessionBase->GetSockPtr();

	auto packet_id = pSessionBase->CountingSendID();
	// 패킷 처리....
	inpPacket->GetReady(packet_id);

	int retval = pSock->Send(inpPacket->m_buf, inpPacket->m_target_sendbytes);
	if (retval == SOCKET_ERROR)
		return false;

	return true;
}

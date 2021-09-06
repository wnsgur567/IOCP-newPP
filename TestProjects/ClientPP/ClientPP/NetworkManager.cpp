#include "base.h"

Implementation_sInstance(NetworkManagerClient);

static int Temp = 1;

// �ӽ�
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
		/*
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

		*/

		printf("�׽�Ʈ ����...\n");

		

		const char* msg = "\
This is Test Message.....\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
aaaaaaaaaaaaaaaaaaaaaa\n\
end\n\n";

		auto pSendpacket = PacketManager::sInstance->GetSendPacketFromPool();
		auto pStream = PacketManager::sInstance->GetSendStreamFromPool();

		int msg_length = strlen(msg);
		pStream->Write(&msg_length, sizeof(msg_length));
		pStream->Write(msg, strlen(msg));

		printf("sendID:%d\n", Temp);
		pSendpacket->Packing(Temp, pStream);	
		

		if (false == Send(pSendpacket))
		{
			// ...
		}
				
		RecvPacketPtr pRecvPacket;
		if (false == Recv(pRecvPacket))
		{
			// ...
		}

		InputMemoryStreamPtr recvStream;
		PacketBase::packetId_t recv_id;
		pRecvPacket->UnPackging(recv_id, recvStream);
			

		printf("recvID:%d\n", recv_id);
		char recv_msg[512];
		ZeroMemory(recv_msg,512);
		
		int recv_msg_length;
		recvStream->Read(&recv_msg_length, sizeof(recv_msg_length));
		recvStream->Read(recv_msg, recv_msg_length);
				
		printf("recv msg : %s\n", recv_msg);

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

	int size = pSock->Recv(&pPacket->m_target_recvbytes, sizeof(RecvPacket::packetSize_t));
	if (size == SOCKET_ERROR)
		return false;

	size = pSock->Recv(pPacket->m_pStream->GetBufferPtr(), pPacket->m_target_recvbytes);
	if (size == SOCKET_ERROR)
		return false;

	pPacket->m_pStream->SetLenth(size);
	// ���� ó��
	outRecvPacket = pPacket;

	return true;
}

bool NetworkManagerClient::Send(SendPacketPtr inpPacket)
{
	TCPSocketPtr pSock = pSessionBase->GetSockPtr();

	auto packet_id = pSessionBase->CountingSendID();
	// ��Ŷ ó��....
	inpPacket->GetReady();
	
	int retval = pSock->Send(inpPacket->m_pStream->GetBufferPtr(), inpPacket->m_target_sendbytes);
	if (retval == SOCKET_ERROR)
		return false;

	return true;
}

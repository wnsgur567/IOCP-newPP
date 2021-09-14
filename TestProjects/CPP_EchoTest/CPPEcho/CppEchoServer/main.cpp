/*
## ���� ���� : 1 v n - IOCP
1. socket()            : ���ϻ���
2. bind()            : ���ϼ���
3. listen()            : ���Ŵ�⿭����
4. accept()            : ������
5. read()&write()
	WIN recv()&send    : ������ �а���
6. close()
	WIN closesocket    : ��������
*/


#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define SERVER_PORT        3500

struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
	SOCKET socket;
	char messageBuffer[MAX_BUFFER];
	int receiveBytes;
	int sendBytes;
};

DWORD WINAPI makeThread(LPVOID hIOCP);

#include "TestNetObject.h"

int main()
{
	/* TestNetObject obj;

	 obj.SetInfo();
	 obj.Print();

	 std::basic_ostringstream<byte> out_stream;
	 obj.Serialize(out_stream);

	 std::basic_istringstream<byte> in_stream;
	 in_stream.str(out_stream.str());
	 TestNetObject obj2;
	 obj2.DeSerialize(in_stream);

	 obj2.Print();*/






	 // Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}

	// 1. ���ϻ���  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return 1;
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	// �Ϸ����� ó���ϴ� ��ü(CP : Completion Port) ����
	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// ��Ŀ������ ����
	// - CPU * 2��
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	int threadCount = systemInfo.dwNumberOfProcessors * 2;
	unsigned long threadId;
	// - thread Handler ����
	HANDLE* hThread = (HANDLE*)malloc(threadCount * sizeof(HANDLE));
	// - thread ����
	for (int i = 0; i < threadCount; i++)
	{
		hThread[i] = CreateThread(NULL, 0, makeThread, &hIOCP, 0, &threadId);
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	SOCKETINFO* socketInfo;
	DWORD receiveBytes;
	DWORD flags;

	while (1)
	{
		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			return 1;
		}

		socketInfo = (struct SOCKETINFO*)malloc(sizeof(struct SOCKETINFO));
		memset((void*)socketInfo, 0x00, sizeof(struct SOCKETINFO));
		socketInfo->socket = clientSocket;
		socketInfo->receiveBytes = 0;
		socketInfo->sendBytes = 0;
		socketInfo->dataBuffer.len = MAX_BUFFER;
		socketInfo->dataBuffer.buf = socketInfo->messageBuffer;
		flags = 0;

		hIOCP = CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, (DWORD)socketInfo, 0);

		// ��ø ��Ĺ�� �����ϰ� �Ϸ�� ����� �Լ��� �Ѱ��ش�.
		if (WSARecv(socketInfo->socket, &socketInfo->dataBuffer, 1, &receiveBytes, &flags, &(socketInfo->overlapped), NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - IO pending Failure\n");
				return 1;
			}
		}
	}

	// 6-2. ���� ��������
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return 0;
}

DWORD WINAPI makeThread(LPVOID hIOCP)
{
	HANDLE threadHandler = *((HANDLE*)hIOCP);
	DWORD receiveBytes;
	DWORD sendBytes;
	DWORD completionKey;
	DWORD flags;
	struct SOCKETINFO* eventSocket;
	while (1)
	{
		// ����� �Ϸ� ���
		if (GetQueuedCompletionStatus(threadHandler, &receiveBytes, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&eventSocket, INFINITE) == 0)
		{
			printf("Error - GetQueuedCompletionStatus Failure\n");
			closesocket(eventSocket->socket);
			free(eventSocket);
			return 1;
		}

		eventSocket->dataBuffer.len = receiveBytes;

		if (receiveBytes == 0)
		{
			closesocket(eventSocket->socket);
			free(eventSocket);
			continue;
		}
		else
		{
			printf("TRACE - (%d bytes)\n", eventSocket->dataBuffer.len);

			byte* byte_ptr = (byte*)eventSocket->messageBuffer;

			int len;
			std::basic_istringstream<byte> read_stream;			
			auto sstr = std::basic_string<byte>();
			sstr.assign(byte_ptr, byte_ptr + 4);
			
			read_stream.str(sstr);
			const byte* ptr = read_stream.str().c_str();
			Utils::ReadFromBinStream(read_stream, len);

			TestNetObject* obj = new TestNetObject();
			Utils::ReadFromBinStream(read_stream, obj);

			obj->Print();

			std::basic_ostringstream<byte> write_stream;
			write_stream.str((byte*)eventSocket->messageBuffer);
			Utils::WriteToBinStream(write_stream, len);
			Utils::WriteToBinStream(write_stream, obj);


			if (WSASend(eventSocket->socket, &(eventSocket->dataBuffer), 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
				}
			}

			printf("TRACE - Send message : %s (%d bytes)\n", eventSocket->dataBuffer.buf, eventSocket->dataBuffer.len);

			memset(eventSocket->messageBuffer, 0x00, MAX_BUFFER);
			eventSocket->receiveBytes = 0;
			eventSocket->sendBytes = 0;
			eventSocket->dataBuffer.len = MAX_BUFFER;
			eventSocket->dataBuffer.buf = eventSocket->messageBuffer;
			flags = 0;

			delete obj;

			if (WSARecv(eventSocket->socket, &(eventSocket->dataBuffer), 1, &receiveBytes, &flags, &eventSocket->overlapped, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
				}
			}
		}
	}
}
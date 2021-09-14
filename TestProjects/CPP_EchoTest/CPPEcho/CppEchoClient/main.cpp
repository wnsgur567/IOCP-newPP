/*
## 소켓 서버 : 1 v n - IOCP
1. socket()            : 소켓생성
2. connect()        : 연결요청
3. read()&write()
    WIN recv()&send    : 데이터 읽고쓰기
4. close()
    WIN closesocket    : 소켓종료
*/

#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define SERVER_IP        "127.0.0.1"
#define SERVER_PORT        3500

struct SOCKETINFO
{
    WSAOVERLAPPED overlapped;
    WSABUF dataBuffer;
    int receiveBytes;
    int sendBytes;
};

int main()
{
    // Winsock Start - winsock.dll 로드
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
    {
        printf("Error - Can not load 'winsock.dll' file\n");
        return 1;
    }

    // 1. 소켓생성
    SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("Error - Invalid socket\n");
        return 1;
    }

    // 서버정보 객체설정
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

    // 2. 연결요청
    if (connect(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Error - Fail to connect\n");
        // 4. 소켓종료
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Server Connected\n* Enter Message\n->");
    }

    SOCKETINFO* socketInfo;
    DWORD sendBytes;
    DWORD receiveBytes;
    DWORD flags;

    while (1)
    {
        // 메시지 입력
        char messageBuffer[MAX_BUFFER];
        int i, bufferLen;
        for (i = 0; 1; i++)
        {
            messageBuffer[i] = getchar();
            if (messageBuffer[i] == '\n')
            {
                messageBuffer[i++] = '\0';
                break;
            }
        }
        bufferLen = i;

        socketInfo = (struct SOCKETINFO*)malloc(sizeof(struct SOCKETINFO));
        memset((void*)socketInfo, 0x00, sizeof(struct SOCKETINFO));
        socketInfo->dataBuffer.len = bufferLen;
        socketInfo->dataBuffer.buf = messageBuffer;

        // 3-1. 데이터 쓰기
        int sendBytes = send(listenSocket, messageBuffer, bufferLen, 0);
        if (sendBytes > 0)
        {
            printf("TRACE - Send message : %s (%d bytes)\n", messageBuffer, sendBytes);
            // 3-2. 데이터 읽기
            int receiveBytes = recv(listenSocket, messageBuffer, MAX_BUFFER, 0);
            if (receiveBytes > 0)
            {
                printf("TRACE - Receive message : %s (%d bytes)\n* Enter Message\n->", messageBuffer, receiveBytes);
            }
        }

    }

    // 4. 소켓종료
    closesocket(listenSocket);

    // Winsock End
    WSACleanup();

    return 0;
}
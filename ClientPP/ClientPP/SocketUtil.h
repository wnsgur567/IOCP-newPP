#pragma once

class SocketUtil
{
public:
	static bool Init();
	static void CleanUp();
	static int	GetLastError();
	static void ReportError(const char* inOperationDesc);
	static TCPSocketPtr CreateTCPSocket();

	
#pragma region select model
	static int Select(const std::vector< TCPSocketPtr >* inReadSet,
		std::vector< TCPSocketPtr >* outReadSet,
		const std::vector< TCPSocketPtr >* inWriteSet,
		std::vector< TCPSocketPtr >* outWriteSet
		/*const vector< TCPSocketPtr >* inExceptSet,
		vector< TCPSocketPtr >* outExceptSet*/);
	// set에 집어넣기
	static fd_set* FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* inSockets);
	// select 된 소켓들 걸러내기
	static void FillVectorFromSet(std::vector< TCPSocketPtr >* outSockets, const std::vector< TCPSocketPtr >* inSockets, const fd_set& inSet);
#pragma endregion

#pragma region completion model
	static HandlePtr CreateIOCP(LPTHREAD_START_ROUTINE inWorkThreadPtr,std::vector<HandlePtr>& outWorkerThreads);
	static HandlePtr LinkIOCPThread(ClientInfoPtr inInfo);
#pragma endregion
};
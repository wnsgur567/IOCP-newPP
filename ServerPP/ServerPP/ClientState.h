#pragma once

class IdleState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : ProtocolSize_t
	{

	};
public:
	virtual IOCPSession::Signal OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr&) override;
	virtual void OnSendCompleted() override;

	static IOCPSession::ClientStatePtr Create();
};



class LobbyState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : ProtocolSize_t
	{

	};
public:
	virtual IOCPSession::Signal OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr&) override;
	virtual void OnSendCompleted() override;

	static IOCPSession::ClientStatePtr Create();
};
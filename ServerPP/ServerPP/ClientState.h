#pragma once

class IdleState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : IOCPSession::Signal
	{

	};
public:
	virtual IOCPSession::Signal OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr) override;
	virtual void OnSendCompleted(IOCPSessionPtr) override;

	static IOCPSession::ClientStatePtr Create();
};

class SignState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : IOCPSession::Signal
	{
		None = 0,

		SignIn = 1 << 0,
		SignOut = 1 << 1,
		SignUp = 1 << 2,
		DeleteAccount = 1 << 3,
	};
protected:
	SignState() : IOCPSession::ClientState(EState::Sign) {}
public:
	virtual IOCPSession::Signal OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr) override;
	virtual void OnSendCompleted(IOCPSessionPtr) override;

	static IOCPSession::ClientStatePtr Create();
};

class LobbyState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : IOCPSession::Signal
	{

	};
public:
	virtual IOCPSession::Signal OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr) override;
	virtual void OnSendCompleted(IOCPSessionPtr) override;

	static IOCPSession::ClientStatePtr Create();
};
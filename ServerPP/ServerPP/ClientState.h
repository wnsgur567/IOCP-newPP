#pragma once

class IdleState : public IOCPSession::ClientState
{
private:
	enum class EProtocol
	{

	};
public:
	virtual OutputMemoryStreamPtr OnRecvCompleted(InputMemoryStreamPtr) override;
	virtual void OnSendCompleted() override;
};

class SignState : public IOCPSession::ClientState
{
private:
	enum class EProtocol
	{

	};
public:
	virtual OutputMemoryStreamPtr OnRecvCompleted(InputMemoryStreamPtr) override;
	virtual void OnSendCompleted() override;
};

class LobbyState : public IOCPSession::ClientState
{
private:
	enum class EProtocol
	{

	};
public:
	virtual OutputMemoryStreamPtr OnRecvCompleted(InputMemoryStreamPtr) override;
	virtual void OnSendCompleted() override;
};
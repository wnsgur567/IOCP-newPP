#pragma once


class SignState : public IOCPSession::ClientState
{
private:
	enum class EProtocol : ProtocolSize_t
	{
		None = 0,

		SignIn = 1 << 0,
		SignOut = 1 << 1,
		SignUp = 1 << 2,
		DeleteAccount = 1 << 3,
	};
protected:
	SignState(IOCPSessionPtr inpOwnerSession) : ClientState(inpOwnerSession) {}
public:
	virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	virtual void OnSendCompleted() override;

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};
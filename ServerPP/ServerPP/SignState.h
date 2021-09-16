#pragma once


class SignState : public IOCPSession::ClientState
{
private:
	using EProtocol = Sign::SignManager::EProtocol;
	using EResult = Sign::SignManager::EResult;
protected:
	EResult m_current_result;
	SignState(IOCPSessionPtr inpOwnerSession) :
		ClientState(inpOwnerSession), m_current_result(EResult::None) {}
public:
	virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	virtual void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;

	void GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol);
	void HandleSignInPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream);
	void HandleSignOutPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream);
	void HandleSignUpPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream);
	void HandleDeleteAccountPacket(NetBase::InputMemoryStreamPtr inpStream, NetBase::OutputMemoryStreamPtr& outpStream);	

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);	
};
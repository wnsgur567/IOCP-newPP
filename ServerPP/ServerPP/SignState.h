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
	virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr) override;
	virtual void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;

	void GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol);	

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};
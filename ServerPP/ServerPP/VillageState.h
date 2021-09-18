#pragma once

class VillageState : public IOCPSession::ClientState
{
private:
	using EProtocol = Village::VillageManager::EProtocol;
	using EResult = Village::VillageManager::EResult;
	EResult m_current_result;
	Village::VillageInfoBasePtr m_pVillageInfo;
protected:
	VillageState(IOCPSessionPtr inOwnerSession)
		: ClientState(inOwnerSession), m_current_result(), m_pVillageInfo() {}
public:
	// ClientState을(를) 통해 상속됨
	virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) override;
	virtual void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis(NetBase::OutputMemoryStreamPtr&) override;
	void GetProtocol(ProtocolSize_t, EProtocol&);

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};
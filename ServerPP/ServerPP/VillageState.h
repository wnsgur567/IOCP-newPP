#pragma once

class VillageState : public IOCPSession::ClientState
{
private:
	using EProtocol = Village::VillageManager::EProtocol;
	using EResult = Village::VillageManager::EResult;
	EResult m_current_result;

	Village::VillageInfoBasePtr m_pVillageInfo;	// ���� �÷��̾ ��ġ�� ���� ����
	SectorPtr m_currentSector;					// ���� �÷��̾ �����ϴ� ����	
protected:
	VillageState(IOCPSessionPtr inOwnerSession)
		: ClientState(inOwnerSession), m_current_result(EResult::None), m_pVillageInfo(nullptr) {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr) override;
	void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;
	void GetProtocol(ProtocolSize_t, EProtocol&);

	

	// ���ڸ� �ൿ
	void HandleAction(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);
	// �̵�
	void HandleMove(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);
	// �̵� + �ൿ
	void HandleMoveAndAction(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};


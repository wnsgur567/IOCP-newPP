#pragma once

class VillageState : public IOCPSession::ClientState
{
private:
	using EProtocol = Village::VillageManager::EProtocol;
	using EResult = Village::VillageManager::EResult;
	EResult m_current_result;

	Village::VillageInfoBasePtr m_pVillageInfo;	// 현재 플레이어가 위치한 마을 정보
	SectorPtr m_currentSector;					// 현재 플레이어가 존재하는 섹터	
protected:
	VillageState(IOCPSessionPtr inOwnerSession)
		: ClientState(inOwnerSession), m_current_result(EResult::None), m_pVillageInfo(nullptr) {}
public:
	void OnRecvCompleted(NetBase::InputMemoryStreamPtr) override;
	void OnSendCompleted() override;
	virtual void OnInitilzed() override;
	virtual void OnChangedToThis() override;
	void GetProtocol(ProtocolSize_t, EProtocol&);

	

	// 제자리 행동
	void HandleAction(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);
	// 이동
	void HandleMove(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);
	// 이동 + 행동
	void HandleMoveAndAction(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&);

	static IOCPSession::ClientStatePtr Create(IOCPSessionPtr);
};


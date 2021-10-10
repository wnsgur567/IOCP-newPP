#pragma once


class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;


class IOCPSession
	: public IOCP_Base::IOCPSessionBase, public std::enable_shared_from_this<IOCPSession>
{
private:
	DERIVED_ENGINE_FRIEND_CODE;
	friend class SignState;
	friend class CharacterSelectState;
	friend class VillageState;
	friend class DungeonState;
public:

	class ClientState
	{
	protected:
		std::weak_ptr<IOCPSession> m_ownerPtr;
	public:
		ClientState(IOCPSessionPtr inpOwnerSession) : m_ownerPtr(inpOwnerSession) {	}
		// Client로부터 요청이 들어온 data Recv 완료 이후 호출
		virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr) = 0;
		// Server에서 Client에게 data Send 완료 이후 호출
		virtual void OnSendCompleted() = 0;
		// 첫 세션 생성 시 state 를 생성하면서 필요한 초기화 작업
		virtual void OnInitilzed() = 0;
		// 다른 state에서 자신의 state 로 변경되었을 때 할 작업
		virtual void OnChangedToThis() = 0;
	};

	using ClientStatePtr = std::shared_ptr<ClientState>;
protected:
	friend class ClientState;
	ClientStatePtr m_current_state;	// 현재 state

	ClientStatePtr m_sign_state;
	ClientStatePtr m_characterSelect_state;
	ClientStatePtr m_village_state;
	ClientStatePtr m_dungeoun_state;
	ClientStatePtr m_chat_state;

	void ChangeState(ClientStatePtr pNextState);
protected:
	bool m_isSigned;
	uint64_t m_session_id;	// session이 생성될 때 부여되는 고유 id
	uint64_t m_user_id;		// id pw 에 대응되는 user 고유 id	
	PlayerInfoPtr m_player;	// character 선택 후 선택된 캐릭터의 정보들
	PlayerPartyInfoPtr m_party;
	Village::VillageInfoBasePtr m_village;	// 
public:
	void SetSign(bool b) { m_isSigned = b; }
	bool GetIsSigned() const { return m_isSigned; }
	void SetSessionID(uint64_t inID) { m_session_id = inID; }
	uint64_t GetSessionID() const { return m_session_id; }
	void SetUserID(uint64_t inID) { m_user_id = inID; }
	uint64_t GetUserID() const { return m_user_id; }
	void SetCharacterInfo(CharacterInfoPtr inInfo) { m_player->SetCharacterInfo(inInfo); }
	PlayerInfoPtr GetPlayerInfo() { return m_player; }
	void SetVillageInfo(Village::VillageInfoBasePtr inVillage) { m_village = inVillage; }
	Village::VillageInfoBasePtr GetVillageInfo() { return m_village; }
protected:
	static IOCP_Base::IOCPSessionBasePtr CreateSession();
protected:

	IOCPSession();
public:
	void Initialze() override;

	bool Recv() override;
	bool Send(NetBase::OutputMemoryStreamPtr pStream) override;
	bool OnCompleteRecv() override;			// Network manager에 의해 완료 시 호출됨
	bool OnCompleteSend() override;			// Network manager에 의해 완료 시 호출됨
	void OnBeforeDisconnected() override;	// Network manager에 의해 완료 시 호출됨
};
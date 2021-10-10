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
		// Client�κ��� ��û�� ���� data Recv �Ϸ� ���� ȣ��
		virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr) = 0;
		// Server���� Client���� data Send �Ϸ� ���� ȣ��
		virtual void OnSendCompleted() = 0;
		// ù ���� ���� �� state �� �����ϸ鼭 �ʿ��� �ʱ�ȭ �۾�
		virtual void OnInitilzed() = 0;
		// �ٸ� state���� �ڽ��� state �� ����Ǿ��� �� �� �۾�
		virtual void OnChangedToThis() = 0;
	};

	using ClientStatePtr = std::shared_ptr<ClientState>;
protected:
	friend class ClientState;
	ClientStatePtr m_current_state;	// ���� state

	ClientStatePtr m_sign_state;
	ClientStatePtr m_characterSelect_state;
	ClientStatePtr m_village_state;
	ClientStatePtr m_dungeoun_state;
	ClientStatePtr m_chat_state;

	void ChangeState(ClientStatePtr pNextState);
protected:
	bool m_isSigned;
	uint64_t m_session_id;	// session�� ������ �� �ο��Ǵ� ���� id
	uint64_t m_user_id;		// id pw �� �����Ǵ� user ���� id	
	PlayerInfoPtr m_player;	// character ���� �� ���õ� ĳ������ ������
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
	bool OnCompleteRecv() override;			// Network manager�� ���� �Ϸ� �� ȣ���
	bool OnCompleteSend() override;			// Network manager�� ���� �Ϸ� �� ȣ���
	void OnBeforeDisconnected() override;	// Network manager�� ���� �Ϸ� �� ȣ���
};
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
	
public:	

	class ClientState
	{
		
	protected:
		std::weak_ptr<IOCPSession> m_ownerPtr;
	public:
		ClientState(IOCPSessionPtr inpOwnerSession)
			: m_ownerPtr(inpOwnerSession) {	}

		// input read �� protocol �κ� ���� ����
		virtual void OnRecvCompleted(NetBase::InputMemoryStreamPtr, NetBase::OutputMemoryStreamPtr&) = 0;
		virtual void OnSendCompleted() = 0;

		// ù ���� ���� �� state �� �����ϸ鼭 �ʿ��� �ʱ�ȭ �۾�
		virtual void OnInitilzed() = 0;
		// �ٸ� state���� �ڽ��� state �� ����Ǿ��� �� �� �۾�
		virtual void OnChangedToThis(NetBase::OutputMemoryStreamPtr&) = 0;
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
	uint64_t m_session_id;	// session�� ������ �� �ο��Ǵ� ���� id
	uint64_t m_user_id;		// id pw �� �����Ǵ� user ���� id	
protected:
	static IOCP_Base::IOCPSessionBasePtr CreateSession();
protected:
	bool m_isSigned;
	IOCPSession();
public:
	void Initialze() override;

	bool IsSigned() const;

	bool Recv() override;
	bool Send(NetBase::OutputMemoryStreamPtr pStream) override;
	bool OnCompleteRecv() override;			// Network manager�� ���� �Ϸ� �� ȣ���
	bool OnCompleteSend() override;			// Network manager�� ���� �Ϸ� �� ȣ���
	void OnBeforeDisconnected() override;	// Network manager�� ���� �Ϸ� �� ȣ���
};
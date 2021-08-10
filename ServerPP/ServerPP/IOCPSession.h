#pragma once

class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;

class IOCPSession
	: public IOCPSessionBase, public std::enable_shared_from_this<IOCPSession>
{
private:
	friend class IOCPNetworkManager;
public:	
	class ClientState
	{
	public:
		enum class EState
		{
			None = 0,

			Sign,
			Game,

			Chat,
		};	
		ClientState();
		virtual OutputMemoryStreamPtr OnRecvCompleted(InputMemoryStreamPtr) = 0;
		virtual void OnSendCompleted() = 0;
		void ChangeState(IOCPSessionPtr pSession, EState inState)
		{
			auto pState = pSession->m_state_map.find(inState);
			if (pSession->m_state_map.end() == pState)
			{	// ���� ��� 
				// �� state ����

			}
			// �ִ� ��� 
			pSession->m_current_state = pState->second;
		}
	};
	using ClientStatePtr = std::shared_ptr<ClientState>;

	/*enum class EState
	{
		None,

		Sign,

		Disconnected
	};*/
protected:
	ClientStatePtr m_current_state;	// ���� state
	std::unordered_map<ClientState::EState, ClientStatePtr> m_state_map;
	//EState m_state;
	bool m_isSigned;
	void Initialze() override;	
public:
	IOCPSession();

	bool IsSigned() const;
	//void SetSigned(bool b);

	bool Recv() override;
	bool Send(OutputMemoryStreamPtr pStream) override;
	bool OnCompleteRecv() override;			// Network manager�� ���� �Ϸ� �� ȣ���
	bool OnCompleteSend() override;			// Network manager�� ���� �Ϸ� �� ȣ���
	void OnBeforeDisconnected() override;	// Network manager�� ���� �Ϸ� �� ȣ���
};
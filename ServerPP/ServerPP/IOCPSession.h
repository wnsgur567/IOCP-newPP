#pragma once

class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;

class IOCPSession
	: public IOCPSessionBase, public std::enable_shared_from_this<IOCPSession>
{
private:
	friend class IOCPNetworkManager;
public:
	class ClientState;
	using ClientStatePtr = std::shared_ptr<ClientState>;
	class ClientState
	{
	public:
		using fpCreateState = ClientStatePtr(*)();
		enum class EState
		{
			None = 0,

			Sign,
			Lobby,
			Village,
			Dungeon,			

			Chat,
		};
	protected:
		EState m_state;
	public:
		ClientState(EState inState) : m_state(inState) {}
		virtual OutputMemoryStreamPtr OnRecvCompleted(InputMemoryStreamPtr) = 0;
		virtual void OnSendCompleted(IOCPSessionPtr) = 0;
		void ChangeState(IOCPSessionPtr pSession, EState inState, fpCreateState inFp)
		{
			auto pState = pSession->m_state_map.find(inState);
			if (pSession->m_state_map.end() == pState)
			{	// 없는 경우 
				// 새 state 생성
				ClientStatePtr newState = inFp();
				pSession->m_state_map.insert({ inState, newState });
				pSession->m_current_state = newState;
			}
			else
			{	// 있는 경우 
				pSession->m_current_state = pState->second;
			}
		}
	};
protected:
	ClientStatePtr m_current_state;	// 현재 state
	std::unordered_map<ClientState::EState, ClientStatePtr> m_state_map;
	bool m_isSigned;
	void Initialze() override;
public:
	IOCPSession();

	bool IsSigned() const;
	//void SetSigned(bool b);

	bool Recv() override;
	bool Send(OutputMemoryStreamPtr pStream) override;
	bool OnCompleteRecv() override;			// Network manager에 의해 완료 시 호출됨
	bool OnCompleteSend() override;			// Network manager에 의해 완료 시 호출됨
	void OnBeforeDisconnected() override;	// Network manager에 의해 완료 시 호출됨
};
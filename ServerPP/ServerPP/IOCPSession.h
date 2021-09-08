#pragma once

class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;

class IOCPSession
	: public IOCPSessionBase, public std::enable_shared_from_this<IOCPSession>
{
private:
	DERIVED_ENGINE_FRIEND_CODE;
	
	friend class SignState;
public:
	using Signal = __int32;

	class ClientState
	{
	public:
		using ProtocolSize_t = __int64;
		using ResultSize_t = __int32;
	protected:
		std::weak_ptr<IOCPSession> m_ownerPtr;
	public:
		ClientState(IOCPSessionPtr inpOwnerSession)
			: m_ownerPtr(inpOwnerSession) {}

		// input read 시 protocol 부분 부터 읽음
		virtual Signal OnRecvCompleted(InputMemoryStreamPtr, __out OutputMemoryStreamPtr&) = 0;
		virtual void OnSendCompleted() = 0;
	};
	using ClientStatePtr = std::shared_ptr<ClientState>;


protected:
	friend class ClientState;
	ClientStatePtr m_current_state;	// 현재 state

protected:
	bool m_isSigned;
	IOCPSession();
	static IOCPSessionBasePtr CreateSession();
public:
	void Initialze() override;

	bool IsSigned() const;
	//void SetSigned(bool b);

	bool Recv() override;
	bool Send(OutputMemoryStreamPtr pStream) override;
	bool OnCompleteRecv() override;			// Network manager에 의해 완료 시 호출됨
	bool OnCompleteSend() override;			// Network manager에 의해 완료 시 호출됨
	void OnBeforeDisconnected() override;	// Network manager에 의해 완료 시 호출됨
};
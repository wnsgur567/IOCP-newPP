#pragma once

class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;

class IOCPSession
	: public IOCPSessionBase, public std::enable_shared_from_this<IOCPSession>
{
private:
	friend class IOCPNetworkManager;
public:
	enum class EState
	{
		None,

		Sign,

		Disconnected
	};
protected:
	EState m_state;
	bool m_isSigned;
	void Initialze() override;	
public:
	IOCPSession();

	EState GetState();
	//void SetState(ESessionState inState);
	bool IsSigned() const;
	//void SetSigned(bool b);

	bool Recv() override;
	bool Send(SendPacketPtr pSendPacket) override;
	bool OnCompleteRecv() override;	// Network manager에 의해 완료 시 호출됨
	bool OnCompleteSend() override; // Network manager에 의해 완료 시 호출됨	
};
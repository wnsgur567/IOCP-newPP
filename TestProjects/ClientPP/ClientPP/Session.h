#pragma once

enum class ESessionState
{
	None,

	Conneted,

	Sign,

	Disconneted,
};


class Session : public SessionBase
{
private:
	friend class NetworkManagerClient;
protected:
	ESessionState m_state;
	
	bool IsSignedIn;
	RecvPacketPtr m_pRecvPacket;
	SendPacketPtr m_pSendPacket;
public:
	Session();
	virtual ~Session();
	
	ESessionState GetState();	
	void SetState(ESessionState inState);	
public:
	bool Recv() override;
	bool Send() override;
};
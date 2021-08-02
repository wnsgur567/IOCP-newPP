#pragma once

class IOCPSession;
using IOCPSessionPtr = std::shared_ptr<IOCPSession>;

class IOCPSession : public IOCPSessionBase
{
	friend class IOCPNetworkManager;
protected:
	bool m_isSigned;
	void Initialze() override;
public:
	IOCPSession();

	bool Recv() override;
	bool Send() override;
	bool OnCompleteRecv() override;
	bool OnCompleteSend() override;

	void SetSigned(bool b);
	bool IsSigned() const;
};
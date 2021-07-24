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

	void SetSigned(bool b);
	bool IsSigned() const;
};
#pragma once


class IOCPSession : public IOCPSessionBase
{
	friend class IOCPNetworkManager;
protected:
	bool m_isSigned;
public:
	IOCPSession();

	void SetSigned(bool b);
	bool IsSigned() const;
};
#include "base.h"



IOCPSession::IOCPSession()
	: IOCPSessionBase(), m_isSigned(false)
{
}

void IOCPSession::SetSigned(bool b)
{
	m_isSigned = b;
}

bool IOCPSession::IsSigned() const
{
	return m_isSigned;
}

IOCPSessionBase::IOCPSessionBase()
{
}

#pragma once

class SocketAddress
{
private:
	sockaddr_in m_sockaddr;
public:
	sockaddr_in& GetSockAddr();
	const sockaddr_in& GetSockAddr() const;
	int GetAddrSize();
	int GetAddrSize() const;
public:
	SocketAddress();
	// port 는 htons 처리한 값을 넣기 (내부서 따로 처리 안함)
	SocketAddress(const u_long inAddress, const u_short inPort);
	SocketAddress(const SocketAddress& inOtherAddr);
	void Print(const char* format) const;
	bool operator==(const SocketAddress& inOther) const
	{
		if (m_sockaddr.sin_family == inOther.m_sockaddr.sin_family &&
			m_sockaddr.sin_addr.s_addr == inOther.m_sockaddr.sin_addr.s_addr &&
			m_sockaddr.sin_port == inOther.m_sockaddr.sin_port)
			return true;

		return false;
	}
};
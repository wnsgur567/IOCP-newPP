#pragma once

class IOCPSessionManager : public SessionManagerBase<IOCPSessionManager>
{
	friend class Singleton;
protected:
	IOCPSessionManager();	
};

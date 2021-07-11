#pragma once

template <typename T>
class AppSingleton : public Singleton<T>
{
public:
	AppSingleton() : Singleton<T>() {}
	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;

	template <typename ret_t>
	virtual ret_t StreamProcess(IOCPSessionPtr inpSession, InputMemoryStreamPtr inpStream) = 0;
};

#pragma once

template <typename T>
class Singleton
{
public:
	static std::unique_ptr<T> sInstance;
protected:
	Singleton()
	{

	}
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	static bool StaticInit()
	{
		sInstance.reset(new T());		
		return true;
	}
	virtual ~Singleton()
	{

	}
public:
	// class 초기화
	virtual bool Initialize(LPVOID) = 0;
	// class 정리
	virtual void Finalize() = 0;
};

#define Implementation_sInstance(className) std::unique_ptr<className> Singleton<className>::sInstance
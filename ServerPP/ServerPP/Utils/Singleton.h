#pragma once

#include<memory>

#ifndef VOID 
#define VOID void
#endif // !VOID

#ifndef LPVOID 
#define LPVOID VOID*
#endif // !LPVOID

namespace MyBase
{

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
		static bool StaticInit() noexcept
		{
			sInstance.reset(new T());
			return true;
		}
		virtual ~Singleton()
		{

		}
	public:
		// class �ʱ�ȭ
		virtual bool Initialize(LPVOID) noexcept = 0;
		// class ����
		virtual void Finalize() noexcept  = 0;
	};
}

#define Implementation_sInstance(className) std::unique_ptr<className> MyBase::Singleton<className>::sInstance
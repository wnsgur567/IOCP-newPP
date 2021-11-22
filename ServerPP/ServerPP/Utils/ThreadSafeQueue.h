#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

template<typename _Ty>
class ThreadSafeQueue
{
private:
	std::mutex mux;
	std::condition_variable cv;

	std::mutex muxQueue;
	std::queue<_Ty> queue;
public:
	void Wait();
	void Push(const _Ty&);
	_Ty Pop();
	bool IsEmpty();
	void Clear();

	void Finalize();
};

template<typename _Ty>
inline void ThreadSafeQueue<_Ty>::Wait()
{
	while (IsEmpty())
	{
		std::unique_lock<std::mutex> ul(mux);
		cv.wait(ul);
	}
}

template<typename _Ty>
inline void ThreadSafeQueue<_Ty>::Push(const _Ty& item)
{
	std::scoped_lock<std::mutex> sl(muxQueue);
	queue.push(item);

	std::unique_lock<std::mutex> ul(mux);
	cv.notify_one();
}

template<typename _Ty>
inline _Ty ThreadSafeQueue<_Ty>::Pop()
{
	std::scoped_lock<std::mutex> sl(muxQueue);
	_Ty ret = queue.front();
	queue.pop();
	return ret;
}

template<typename _Ty>
inline bool ThreadSafeQueue<_Ty>::IsEmpty()
{
	std::scoped_lock<std::mutex> sl(muxQueue);
	return queue.empty();
}

template<typename _Ty>
inline void ThreadSafeQueue<_Ty>::Clear()
{
	std::scoped_lock<std::mutex> sl(muxQueue);
	while (!queue.empty())
	{
		queue.pop();
	}
}

template<typename _Ty>
inline void ThreadSafeQueue<_Ty>::Finalize()
{
	Clear();

	std::scoped_lock<std::mutex> sl(mux);
	cv.notify_all();
}



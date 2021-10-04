#pragma once

class ThreadPool
{
private:
	mutable std::mutex m_Mutex;
	std::queue<std::packaged_task<void()>> m_Tasks;
	std::vector<std::thread> m_Threads;
	std::condition_variable_any m_Condition;
	bool m_Terminate = false;

public:
	ThreadPool(unsigned int threads = std::thread::hardware_concurrency());
	~ThreadPool();

	void Start(unsigned int threads = std::thread::hardware_concurrency());
	void Reset();
	void Stop();
	std::future<void> Enqueue(std::function<void()>&& job);
	bool IsBusy();
};

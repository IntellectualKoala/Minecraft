#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int threads) {
	Start(threads);
}

ThreadPool::~ThreadPool() {
	Stop();
}

void ThreadPool::Start(unsigned int threads) {
	for (unsigned int i = 0; i < threads; ++i) {
		m_Threads.emplace_back(std::thread([&]() {
			while (true) {
				std::packaged_task<void()> job;

				{
					std::unique_lock lock(m_Mutex);

					m_Condition.wait(lock, [&] {
						return (!m_Tasks.empty() || m_Terminate);
					});

					if (m_Terminate)
						break;

					job = std::move(m_Tasks.front());
					m_Tasks.pop();
				}

				if (!job.valid())
					return;

				job();
			}
		}));
	}
}

void ThreadPool::Reset() {
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		std::queue<std::packaged_task<void()>>().swap(m_Tasks);
	}

	m_Condition.notify_all();
}

void ThreadPool::Stop() {
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Terminate = true;
	}

	m_Condition.notify_all();

	for (auto& thread : m_Threads)
		thread.join();
}

std::future<void> ThreadPool::Enqueue(std::function<void()>&& job) {
	std::packaged_task<void()> task(job);
	auto future = task.get_future();

	{
		std::lock_guard lock(m_Mutex);
		m_Tasks.emplace(std::move(task));
	}

	m_Condition.notify_one();
	return future;
}

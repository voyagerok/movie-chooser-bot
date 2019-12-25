#include "threadpool.h"

#include "log.h"

ThreadPool::ThreadPool() : m_interruptFlag(false)
{
    auto threads = std::thread::hardware_concurrency();
    for (size_t i = 0; i < threads; ++i) {
        m_workerThreads.emplace_back(&ThreadPool::thread_loop, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lk(m_queueLock);
        m_interruptFlag = true;
        m_condvar.notify_all();
    }
    for (size_t i = 0; i < m_workerThreads.size(); ++i) {
        m_workerThreads[i].join();
    }
}

void ThreadPool::thread_loop()
{
    for (;;) {
        Task task;
        {
            std::unique_lock<std::mutex> lk(m_queueLock);
            m_condvar.wait(lk, [this]() -> bool { return m_interruptFlag || !m_tasks.empty(); });
            if (m_interruptFlag) {
                LOGTRACE << "thread 0x" << std::hex << std::this_thread::get_id()
                         << ": interrupt requested";
                break;
            }
            task = m_tasks.front();
            m_tasks.pop();
        }
        if (task) { task(); }
    }
}

ThreadPool &ThreadPool::instance()
{
    static ThreadPool instance;
    return instance;
}

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <queue>

class ThreadPool
{
public:
    static ThreadPool &instance();

public:
    ~ThreadPool();

    template<class F, class... Args, class R = std::result_of_t<F(Args...)>>
    std::future<R> add(F &&f, Args &&... args)
    {
        auto task = std::make_shared<std::packaged_task<R()>>([=] { return f(args...); });
        auto future = task->get_future();
        {
            std::lock_guard<std::mutex> lk(m_queueLock);
            m_tasks.push([=]() { (*task)(); });
            m_condvar.notify_all();
        }
        return future;
    }

private:
    ThreadPool();
    using Task = std::function<void()>;
    void thread_loop();

private:
    std::vector<std::thread> m_workerThreads;
    std::condition_variable m_condvar;
    std::mutex m_queueLock;
    std::queue<Task> m_tasks;
    bool m_interruptFlag;
};

#endif // THREADPOOL_H

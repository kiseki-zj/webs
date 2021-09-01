#ifndef __THREADPOOL__
#define __THREADPOOL__
#include <pthread.h>
#include <functional>
#include <memory>
#include <vector>
#include <queue>
struct ThreadPoolTask {
    std::function<void(std::shared_ptr<void>)> fun;
    std::shared_ptr<void> args;
    ThreadPoolTask(std::function<void(std::shared_ptr<void>)> _fun, std::shared_ptr<void> _args):fun(_fun), args(_args) {}
};

class ThreadPool {
private:
    static size_t num_threads;
    static size_t queue_size;
    static std::vector<pthread_t> threads;
    static std::queue<ThreadPoolTask> queue;
    static pthread_mutex_t lock;
    static pthread_cond_t cond;
    static int started;
    static void *Run(void* args);
    static size_t count;
    static int shutdown;
public:
    ThreadPool(size_t _num_threads, size_t _queue_size);
    static int add_task(ThreadPoolTask task);
    static void DestroyThreadPool();
    static void FreeThreadPool();
    static int get_started() { return started; }
};

#endif
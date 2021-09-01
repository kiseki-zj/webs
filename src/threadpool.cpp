#include "threadpool.h"
#include <stdio.h>
#include <unistd.h>
using namespace std;

size_t ThreadPool::num_threads;
size_t ThreadPool::queue_size;
std::vector<pthread_t> ThreadPool::threads;
std::queue<ThreadPoolTask> ThreadPool::queue;
pthread_mutex_t ThreadPool::lock;
pthread_cond_t ThreadPool::cond;
int ThreadPool::started;
size_t ThreadPool::count;
int ThreadPool::shutdown;

ThreadPool::ThreadPool(size_t _num_threads, size_t _queue_size) {
    num_threads = _num_threads;
    queue_size = _queue_size;
    started = 0;
    count = 0;
    shutdown = 0;
    threads.resize(num_threads);
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "mutex_init error\n");
    }
    if (pthread_cond_init(&cond, NULL) != 0) {
        fprintf(stderr, "cond_init error\n");
    }
    
    for (size_t i = 0; i < num_threads; ++i) {
        if (pthread_create(&threads[i], NULL, Run, (void*)(0)) != 0) {
            printf("Create thread failed");
            break;
        }
        else {
            printf("Create thread %d, tid=%d\n", i, threads[i]);
        }
        ++started;
    }
}

int ThreadPool::add_task(ThreadPoolTask task) {
    pthread_mutex_lock(&lock);
    if (queue.size() >= queue_size) {
        printf("Task queue is full\n");
        pthread_mutex_unlock(&lock);
        return -1;
    }
    queue.push(task);
    printf("A task has been added to queue, ");
    count++;
    printf("now we have %d tasks\n", count);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
}

void ThreadPool::DestroyThreadPool() {
    printf("Threadpool is shuting down...\n");
    shutdown = 1;
    //pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&cond);
    //pthread_mutex_lock(&lock);
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("all threads has exited\n");
    FreeThreadPool();
}

void ThreadPool::FreeThreadPool() {
    if (started > 0) {
        printf("Fail to destroy pool:%d threads remaining\n", started);
    }
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}

void* ThreadPool::Run(void* args) {
    while (1) {
        //sleep(2);
        pthread_mutex_lock(&lock);
        while (count == 0 && shutdown == 0) {
            pthread_cond_wait(&cond, &lock);
        }
        if (shutdown) {
            pthread_mutex_unlock(&lock);
            started--;
            printf("thread %d is exiting...\n", pthread_self());
            pthread_exit(NULL);
        }
        ThreadPoolTask task(queue.front());
        queue.front().fun = NULL;
        queue.front().args.reset();
        queue.pop();
        count--;
        
        printf("Now thread %d got a task, there're %d tasks in queue\n", pthread_self(), count);
        printf("thread %d is doing:\n", pthread_self());
        pthread_mutex_unlock(&lock);
        task.fun(task.args);     
    }
}
#include "threadpool.h"
#include <string>
#include <unistd.h>
using namespace std;
const size_t NUM_THREADS = 4;
const size_t QUEUE_SIZE = 1024;
struct node {
    int a;
    string s;
    node(int _a, string _s):a(_a), s(_s){}
};
void fuck(shared_ptr<void> p) {
    shared_ptr<node> ptr = static_pointer_cast<node>(p);
    printf("fuck %s %d times!!!\n", ptr->s, ptr->a);
}
int main() {
    shared_ptr<ThreadPool>pool(new ThreadPool(NUM_THREADS, QUEUE_SIZE));
    if (pool->get_started() != NUM_THREADS) {
        return 1;
    }
    shared_ptr<node> ptr1(new node(5, string("bitch")));
    shared_ptr<node> ptr2(new node(4, string("caonm")));
    shared_ptr<node> ptr3(new node(4, string("caonm")));
    shared_ptr<node> ptr4(new node(4, string("caonm")));
    shared_ptr<node> ptr5(new node(4, string("caonm")));
    shared_ptr<node> ptr6(new node(4, string("caonm")));
    shared_ptr<node> ptr7(new node(4, string("caonm")));
    shared_ptr<node> ptr8(new node(4, string("caonm")));
    shared_ptr<node> ptr9(new node(4, string("caonm")));
    shared_ptr<node> ptr0(new node(4, string("caonm")));
    pool->add_task(ThreadPoolTask(fuck, ptr1));
    pool->add_task(ThreadPoolTask(fuck, ptr2));
    pool->add_task(ThreadPoolTask(fuck, ptr3));
    pool->add_task(ThreadPoolTask(fuck, ptr4));
    pool->add_task(ThreadPoolTask(fuck, ptr5));
    pool->add_task(ThreadPoolTask(fuck, ptr6));
    pool->add_task(ThreadPoolTask(fuck, ptr7));
    pool->add_task(ThreadPoolTask(fuck, ptr8));
    pool->add_task(ThreadPoolTask(fuck, ptr9));
    pool->add_task(ThreadPoolTask(fuck, ptr0));
    sleep(5);
    pool->DestroyThreadPool();
    sleep(10);
}
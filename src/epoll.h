#include <sys/epoll.h>

class Epoll {
    static int Epoll_init();
    static int Epoll_wait();
    static int Epoll_add();
    static int Epoll_mod();
};
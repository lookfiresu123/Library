#ifndef _H_THREAD_POOL
#define _H_THREAD_POOL 1

#include <pthread.h>
#include <queue>
#include <iostream>
#include <vector>
#include <cstdio>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

using std::queue;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

// 保护标准输出的互斥锁
static pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;

// 任务基类
class Task {
public:
    Task();
    virtual ~Task();
    virtual void run() = 0;
    virtual void showTask() = 0;
};

// 计算斐波那契数列的派生任务类
class FibTask : public Task {
public:
    typedef long long int LLI;
    FibTask(int n);
    ~FibTask();
    virtual void run();
    virtual void showTask();
private:
    LLI innerFib(LLI n);
    LLI n_;
};

// 任务队列类
class WorkQueue {
public:
    WorkQueue();                    // 构造函数，初始化互斥锁和条件变量
    ~WorkQueue();                   // 析构函数，销毁互斥锁和条件变量（由于此时任务队列为空，因此不用销毁队列）
    Task *getNextTask();            // 获取队列中的下一个任务
    void addTask(Task *t);          // 加入新任务到队列中
    void finish();                  // 标记队列的工作已经结束，但不代表队列的生命周期结束
    bool empty();                   // 判断队列是否为空
    int size();                     // 判断队列中还有多少个任务
private:
    queue<Task *> tasks;            // 任务队列
    bool finished;                  // 标记任务队列的生命周期是否已经结束
    pthread_mutex_t qmtx;           // 守护队列的互斥锁，同时保护条件状态的改变
    pthread_cond_t wcond;           // 条件变量，当队列为空时wait(),当队列不为空时notify()
};

// 线程池类
class ThreadPool {
public:
    ThreadPool(int n);              // 构造函数，用于分配线程池内存，并创建n个线程
    ~ThreadPool();                  // 析构函数，等待线程池中线程的结束，之后删除它们
    void addTask(Task *t);          // 增加一个任务到任务队列中
    void finish();                  // 结束线程池的运行，即结束任务队列的运行，但不代表线程池的生命周期结束
    bool hasWork();                 // 判断线程池的任务队列是否为空
    static void *getWork(void *param);     // 线程不断的从任务队列中获取任务，直到任务队列为空

private:
    unsigned int thread_count;      // 当前线程池中的线程数量
    WorkQueue work_queue;           // 线程池中的任务队列
    vector<pthread_t> threads;      // 线程容器
};


#endif

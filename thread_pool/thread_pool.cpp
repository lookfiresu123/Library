#include "thread_pool.h"

// 任务类
Task::Task() {}
Task::~Task() {}

typedef long long int LLI;

// 计算斐波那契数列的派生任务类
FibTask::FibTask(int n) : Task(), n_(n) {}
FibTask::~FibTask() {
    pthread_mutex_lock(&console_mutex);
    printf(ANSI_COLOR_RED "Tid(%lld) - fibd(%lld) being deleted" ANSI_COLOR_RESET "\n", static_cast<LLI>(pthread_self()), n_);
    pthread_mutex_unlock(&console_mutex);
}

LLI FibTask::innerFib(LLI n) {
    if (n <= 1) return 1;
    return innerFib(n-1) + innerFib(n-2);
}

void FibTask::run() {
    LLI val = innerFib(n_);
    pthread_mutex_lock(&console_mutex);
    printf(ANSI_COLOR_GREEN "The result of Fibd %lld = %lld" ANSI_COLOR_RESET "\n", n_, val);
    pthread_mutex_unlock(&console_mutex);
}

void FibTask::showTask() {
    pthread_mutex_lock(&console_mutex);
    printf(ANSI_COLOR_BLUE "Thread %lld computing fibonacci %lld" ANSI_COLOR_RESET "\n", static_cast<LLI>(pthread_self()), n_);
    pthread_mutex_unlock(&console_mutex);
}

// 任务队列类
WorkQueue::WorkQueue() {
    pthread_mutex_init(&qmtx, nullptr);
    pthread_cond_init(&wcond, nullptr);
}

WorkQueue::~WorkQueue() {
    // cout << "remain " << size() << " tasks" << endl;
    pthread_mutex_destroy(&qmtx);
    pthread_cond_destroy(&wcond);
}

Task *WorkQueue::getNextTask() {
    Task *t = nullptr;
    // 由于同一时间只能有一个线程访问任务队列，因此线程访问队列前需要先加锁
    pthread_mutex_lock(&qmtx);
    if (!finished) {
        // 利用条件变量使线程只有在队列不为空时才进入临界区，否则睡眠
        while (tasks.empty()) pthread_cond_wait(&wcond, &qmtx);
        // 当队列不为空时，取出第一个任务并执行
        t = tasks.front();
        if(t) {
            tasks.pop();
            // t->showTask();
        }
    }
    pthread_mutex_unlock(&qmtx);    // 解互斥锁
    return t;
}

void WorkQueue::addTask(Task *t) {
    if (!finished) {
        pthread_mutex_lock(&qmtx);
        tasks.push(t);
        pthread_cond_signal(&wcond);    // 当队列中有任务时立即唤醒阻塞中的进程
        pthread_mutex_unlock(&qmtx);
    }
}

void WorkQueue::finish() {
    pthread_mutex_lock(&qmtx);
    if (!finished) {
        finished = true;
        pthread_cond_broadcast(&wcond);    // 唤醒所有阻塞中的进程
    }
    pthread_mutex_unlock(&qmtx);
}

bool WorkQueue::empty() {
    return tasks.empty();
}

int WorkQueue::size() {
    return tasks.size();
}

ThreadPool::ThreadPool(int n) : thread_count(n), threads(thread_count) {
    int rc;
    // 创建线程
    for (unsigned int i = 0 ; i < thread_count ; ++i) {
        if ((rc = pthread_create(&threads[i], nullptr, getWork, &work_queue))) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
}

ThreadPool::~ThreadPool() {
    for (unsigned int i = 0 ; i < thread_count ; ++i)
        work_queue.addTask(nullptr);
    work_queue.finish();
    // 等待线程的结束
    for (unsigned int i = 0 ; i < thread_count ; ++i) {
        pthread_join(threads[i], nullptr);
    }
}

void ThreadPool::addTask(Task *t) {
    work_queue.addTask(t);
}

void ThreadPool::finish() {
    work_queue.finish();
}

bool ThreadPool::hasWork() {
    return !work_queue.empty();
}

// 线程不断的从任务队列中获取任务，直到任务队列为空
void *ThreadPool::getWork(void *param) {
    Task *mw = nullptr;
    WorkQueue *wq = static_cast<WorkQueue *>(param);
    while ((mw = wq->getNextTask())) {
        mw->showTask();
        mw->run();
        // cout << "remain " << wq->size() << " tasks" << endl;
        delete mw;
    }
    // pthread_exit(nullptr);
    return nullptr;
}




#include "thread_pool.h"
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using std::srand;
using std::time;
using std::rand;

int main() {
    // 创建一个线程池
    ThreadPool *tp = new ThreadPool(10);

    // 创建并运行100个工作
    srand(time(0));
    for (int i = 0 ; i < 100 ; ++i) {
        int rv = rand() % 10;
        tp->addTask(new FibTask(rv));
        // cout << "i = " << i << endl;
    }
    sleep(1);
    delete tp;
    return 0;
}

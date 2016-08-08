// 停车场设计流程：
// 1. main函数
// 2. 初始化停车场数据模块
// 3. 创建m个停车线程、n个取车线程、1个监控线程（pthread_create）
// 4. 等待指定线程的终止（pthread_join）
// 5. 结束

#include "parking.h"

int main() {
    int k;
    cout << "Give the capacity of car park" << endl;
    cin >> k;
    Park carpark(k);

    // 创造M个停车线程和N个取车线程
    vector<pthread_t> putcar(M, 0), getcar(N, 0);
    pthread_t monitor_driver;
    for (int i = 0 ; i < M ; ++i)
        pthread_create(&putcar[i], nullptr, Park::car_in_handler, static_cast<void *>(&carpark));
    for (int i = 0 ; i < N ; ++i)
        pthread_create(&getcar[i], nullptr, Park::car_out_handler, static_cast<void *>(&carpark));

    // 创建一个监视器线程
    pthread_create(&monitor_driver, nullptr, Park::monitor, static_cast<void *>(&carpark));

    for (int i = 0 ; i < M ; ++i)
        pthread_join(putcar[i], nullptr);
    for (int i = 0 ; i < N ; ++i)
        pthread_join(putcar[i], nullptr);
    pthread_join(monitor_driver, nullptr);

    return 0;
}

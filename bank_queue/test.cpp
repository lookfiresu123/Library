#include "bank_queue.h"

int main() {
    srand(static_cast<unsigned int>(time(0)));  // 使用当前时间作为随机种子
    int total_service_time = 240;               // 每分钟的服务次数
    int window_num = 4;                         // 服务窗口个数
    int simulate_num = 100000;                  // 模拟次数

    QueueSystem system(total_service_time, window_num);
    system.simulate(simulate_num);

    // 输出每个客户在银行停留的平均时间
    cout << "The average time of customer stay in bank: " << system.getAvgStayTime() << endl;
    // 输出平均每分钟有多少个客户抵达银行
    cout << "The number of customer arrive bank per minute: " << system.getAvgCustomers() << endl;

    return 0;
}

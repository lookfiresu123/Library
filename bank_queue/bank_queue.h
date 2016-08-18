#ifndef _H_BANK_QUEUE
#define _H_BANK_QUEUE 1

#include <pthread.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

#define RANDOM_PARAMETER 100

// 随机数类
class Random {
public:
    // 在[0,max]之间生成一个服从均匀分布的随机值
    static double uniform(double max = 1) {
        return (static_cast<double>(rand()) / RAND_MAX) * max;
    }
};

// 客户类
struct Customer {
public:
    Customer(int arrive_time_ = 0, int duration_ = Random::uniform(RANDOM_PARAMETER));
    int arrive_time;
    int duration;
    Customer *next;
};

// 服务窗口的状态
enum WindowStatus { SERVICE, IDLE };

// 服务窗口类
class ServiceWindow {
public:
    inline ServiceWindow();
    inline bool isIdle() const;
    inline void serveCustomer(Customer &customer_);
    inline void setBusy();
    inline void setIdle();
    inline int getCustomerArriveTime() const;
    inline int getCustomerDuration() const;
private:
    Customer customer;
    WindowStatus window_status;
};

// 事件类
struct Event {
    Event(int occur_time_ = Random::uniform(RANDOM_PARAMETER), int event_type_ = -1);
    int occur_time;
    int event_type; // -1表示有客户到达银行，>=0表示有客户离开银行，且从servicewindow[event_type]离开
    Event *next;
};

// 对列类（既表示客户队列，也表示事件队列）
template <typename T>
class Queue {
public:
    // 构造函数，创建一个带有头结点的链式队列
    Queue() {
        front = new T;
        if (!front) exit(-1);
        front->next = nullptr;
        rear = front;
    }

    // 析构函数，销毁并释放内存
    ~Queue() {
        clearQueue();
        delete front;
        front = nullptr;
        rear = nullptr;
    }

    // 清空队列
    void clearQueue() {
        T *temp_node;
        while (front->next) {
            temp_node = front->next;
            front->next = temp_node->next;
            delete temp_node;
        }

        front->next = nullptr;
        rear = front;
    }

    // 入队
    T *enqueue(T &node) {
        T *new_node = new T;
        if (!new_node)  exit(-1);
        // 入队时，传递节点指针，外部数据不应该由此类进行管理，所以将数据拷贝一份
        *new_node = node;
        rear->next = new_node;
        rear = new_node;
        return front;
    }

    // 出队
    T *dequeue() {
        // 如果队列空，则返回空指针
        if (!front->next)   return nullptr;
        T *temp_node = front->next;
        front->next = temp_node->next;
        // 如果队列中只有一个节点，那么记得将队尾节点指针置为头结点
        if (rear == temp_node)  rear = front;
        return temp_node;
    }

    // 事件入队
    T *orderEnqueue(Event &event) {
        Event *temp = new Event;
        if (!temp)  exit(-1);
        *temp = event;
        // 若事件列表中没有事件，则把temp事件插入
        if (!front->next) {
            enqueue(*temp);
            return front;
        }
        Event *temp_event_list = front;
        // 按时间顺序插入到链表中的合适位置
        while (temp_event_list->next && temp_event_list->occur_time < event.occur_time)
            temp_event_list = temp_event_list->next;
        temp->next = temp_event_list->next;
        temp_event_list->next = temp;

        return front;
    }

    // 获得队列长度
    int length() {
        T *temp_node = front->next;
        int length = 0;
        while (temp_node) {
            temp_node = temp_node->next;
            ++length;
        }
        return length;
    }

private:
    T *front;                       // 头结点
    T *rear;                        // 尾结点
};

// 银行队列系统的模拟
class QueueSystem {
public:
    QueueSystem(int total_service_time_, int window_num_);  // 初始化队列系统
    ~QueueSystem();                                         // 销毁队列系统
    void simulate(int simulate_num);                        // 启动模拟
    double getAvgStayTime();                                // 获取每个客户的平均滞留时间
    double getAvgCustomers();                               // 获取每个窗口每分钟的平均客户数
private:
    double run();               // 系统开始运行，不断消耗事件表，当消耗完成时结束运行
    void init();                // 系统开启运行，初始化事件链表， 部署第一个事件
    void end();                 // 系统运行结束，将所有服务窗口置空闲，并清空用户的等待队列和事件列表
    int getIdleServiceWindow(); // 获得空闲窗口索引
    void customerArrived();     // 处理客户到达事件
    void customerDeparture();   // 处理客户的离开事件
    int window_num;             // 服务窗口的总数
    int total_service_time;     // 总的营业时间
    int total_customer_stay_time;     // 客户逗留总时间
    int total_customer_num;     // 客户总数

    // 核心成员
    ServiceWindow*      windows;        // 服务窗口链表
    Queue<Customer>     customer_list;  // 客户等待队列
    Queue<Event>        event_list;     // 事件队列
    Event*              current_event;  // 当前事件

    // 给外部调用的结果
    double avg_customers;               // 每个窗口每分钟的平均客户数
    double avg_stay_time;               // 每个客户的平均滞留时间

};

#endif

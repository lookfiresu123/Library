#include "bank_queue.h"

// 客户类
Customer::Customer(int arrive_time_, int duration_) :
    arrive_time(arrive_time_), duration(duration_), next(nullptr) {}

// 服务窗口类
ServiceWindow::ServiceWindow() {
    window_status = IDLE;
}

bool ServiceWindow::isIdle() const {
    if (window_status == IDLE)  return true;
    else                        return false;
}

void ServiceWindow::serveCustomer(Customer &customer_) {
    customer = customer_;
}

void ServiceWindow::setBusy() {
    window_status = SERVICE;
}

void ServiceWindow::setIdle() {
    window_status = IDLE;
}

int ServiceWindow::getCustomerArriveTime() const {
    return customer.arrive_time;
}

int ServiceWindow::getCustomerDuration() const {
    return customer.duration;
}

// 事件类
Event::Event(int occur_time_, int event_type_) :
    occur_time(occur_time_), event_type(event_type_), next(nullptr) {}

// 队列系统类
QueueSystem::QueueSystem(int total_service_time_, int window_num_) :
    window_num(window_num_), total_service_time(total_service_time_),
    total_customer_stay_time(0), total_customer_num(0) {
    windows = new ServiceWindow[window_num];
}

QueueSystem::~QueueSystem() {
    delete[] windows;
}

void QueueSystem::simulate(int simulate_num) {
    double sum = 0;
    for (int i = 0 ; i < simulate_num ; ++i)
        sum += run();   // 每一遍运行，我们都要增加在这一次模拟中，顾客逗留了多久
    // 计算平均逗留时间
    avg_stay_time = static_cast<double>(sum) / simulate_num;
    // 计算每分钟平均顾客数
    avg_customers = static_cast<double>(total_customer_num) / (total_service_time * simulate_num);
}

double QueueSystem::getAvgStayTime() {
    return avg_stay_time;
}

double QueueSystem::getAvgCustomers() {
    return avg_customers;
}

void QueueSystem::init() {
    Event *event = new Event();
    current_event = event;
}

double QueueSystem::run() {
    init();
    while (current_event) {
        if (current_event->event_type == -1)
            customerArrived();
        else
            customerDeparture();
        delete current_event;
        current_event = event_list.dequeue();
    }
    end();
    return static_cast<double>(total_customer_stay_time) / total_customer_num;
}

void QueueSystem::end() {
    for (int i = 0 ; i < window_num ; ++i)
        windows[i].setIdle();
    customer_list.clearQueue();
    event_list.clearQueue();
}

void QueueSystem::customerArrived() {
    ++total_customer_num;
    // 生成下一个客户到达事件
    int intertime = Random::uniform(100);   // 下一个顾客到达的时间间隔，我们假设100分钟内一定会出现一个顾客
    // 下一个顾客的到达时间 = 当前时间的发生时间 + 下一个顾客到达的时间间隔
    int time = current_event->occur_time + intertime;
    Event temp_event(time);
    // 如果下一个顾客的到达时间小于服务的总时间，就把这个事件插入到事件列表中, 同时将这个顾客加入到
    // customer_list 进行排队
    if (time < total_service_time)
        event_list.orderEnqueue(temp_event);

    // 处理当前事件中到达的顾客
    Customer *customer = new Customer(current_event->occur_time);
    if (!customer)
        exit(-1);
    customer_list.enqueue(*customer);

    // 如果当前窗口有空闲窗口，那么直接将这个顾客送入服务窗口
    int idleIndex = getIdleServiceWindow();
    if (idleIndex >= 0) {
        customer = customer_list.dequeue();
        windows[idleIndex].serveCustomer(*customer);
        windows[idleIndex].setBusy();
        // 离开事件的发生时间 = 当前时间事件的发生时间 + 服务时间
        Event temp_event(current_event->occur_time + customer->duration, idleIndex);
        event_list.orderEnqueue(temp_event);
    }
    delete customer;
}

int QueueSystem::getIdleServiceWindow() {
    for (int i = 0 ; i < window_num ; ++i)
        if (windows[i].isIdle())
            return i;
    return -1;
}

void QueueSystem::customerDeparture() {
    // 如果离开事件的发生时间比中服务时间大，我们就不需要做任何处理
    if (current_event->occur_time < total_service_time) {
        // 顾客逗留时间 = 当前顾客离开时间 - 顾客的到达时间
        total_customer_stay_time += current_event->occur_time - windows[current_event->event_type].getCustomerArriveTime();
        // 如果队列中有人等待，则立即服务等待的顾客
        if (customer_list.length()) {
            Customer *customer = customer_list.dequeue();
            windows[current_event->event_type].serveCustomer(*customer);
            // 离开事件
            Event temp_event(current_event->occur_time + customer->duration, current_event->event_type);
            event_list.orderEnqueue(temp_event);
            delete customer;
        } else
            windows[current_event->event_type].setIdle();  // 如果队列没有人，且当前窗口的顾客离开了，则这个窗口是空闲的
    }
}

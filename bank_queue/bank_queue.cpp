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


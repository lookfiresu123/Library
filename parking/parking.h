#ifndef _H_PARKING
#define _H_PARKING 1

#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <memory>
#include <queue>
#include <string>

using namespace std;

static int id = 0;

#define ONE_SECOND 1000
#define PERIOD 10
#define M 2
#define N 2
#define NUM_THREADS (M+N)


// 车类
class Car {
public:
    Car(int ID_, const string &brands_, const string &area_);
    void print();
private:
    int ID;         // 车牌号
    string brands;  // 车的品牌
    string area;    // 车的地区
};

// 停车场类
class Park {
public:
    Park(int capacity_);
    static void *car_in_handler(void *context);
    static void *car_out_handler(void *context);
    static void *monitor(void *context);
private:
    int capacity;                   // 停车场的车辆容量
    int size;                       // 停车场的现有车辆数
    vector<shared_ptr<Car>> buffer; // 停车场的停车位缓冲区
    int next_in;                    // 下一辆进来的车的停车位置
    int next_out;                   // 下一辆取走的车的停车位置
    int cars_in;                    // 记录停车场进入的车辆总和
    int cars_out;                   // 记录从停车场开出去的车辆总和
    pthread_mutex_t lock;           // 互斥量，保证该结构中的数据被线程互斥的访问
    pthread_cond_t space;           // 条件变量，描述停车场是否有空位置
    pthread_cond_t car;             // 条件变量，描述停车场是否有车
    pthread_barrier_t bar;          // 线程屏障
};


#endif

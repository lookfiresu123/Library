#include "parking.h"

string Brands[] = {"BENZE","BMW","TOYOTA","HONDA","JEEP"};
string Ereas[] = {"NEW YORK","SHANGHI","TOKYO","LONDON"};

// 车类
Car::Car(int ID_ ,const string &brands_, const string &area_) :
    ID(ID_), brands(brands_), area(area_) {}

void Car::print() {
    cout << ID << " : " <<  brands << " : " << area << endl;
}

// 停车场类
Park::Park(int capacity_) : capacity(capacity_), size(0),
    buffer(capacity, nullptr), next_in(0), next_out(0),
    cars_in(0), cars_out(0) {
    srand(static_cast<unsigned int>(getpid()));

    pthread_mutex_init(&lock, nullptr);     // 初始化停车场的锁
    pthread_cond_init(&space, nullptr);     // 初始化描述停车场是否有空位的条件变量
    pthread_cond_init(&car, nullptr);       // 初始化描述停车场是否有车的条件变量
    pthread_barrier_init(&bar, nullptr, NUM_THREADS);   // 初始化线程屏障
}

void *Park::car_in_handler(void *context) {
    Park *tmp = static_cast<Park *>(context);
    unsigned int seed;
    pthread_barrier_wait(&tmp->bar); // 表明线程已完成工作，等待其它线程赶来
    while (true) {
        usleep(rand_r(&seed) % ONE_SECOND); // 将线程随机挂起一段时间，模仿车辆到来的随机性
        pthread_mutex_lock(&tmp->lock);      // 加锁，互斥的访问停车场

        // 循环等待，直到有停车位
        while (tmp->size == tmp->capacity)
            pthread_cond_wait(&tmp->space, &tmp->lock);

        shared_ptr<Car> per_car(new Car(id++, Brands[rand()%5], Ereas[rand()%4]));
        tmp->buffer[tmp->next_in] = per_car;
        ++tmp->size;
        ++tmp->next_in;
        tmp->next_in %= tmp->capacity;
        ++tmp->cars_in;

        pthread_cond_signal(&tmp->car);      // 发送有车可取的信号
        pthread_mutex_unlock(&tmp->lock);    // 解锁
    }
    return nullptr;
}

void *Park::car_out_handler(void *context) {
    Park *tmp = static_cast<Park *>(context);
    unsigned int seed;
    pthread_barrier_wait(&tmp->bar);

    while (true) {
        usleep(rand_r(&seed) % ONE_SECOND);
        pthread_mutex_lock(&tmp->lock);

        while (tmp->size == 0)
            pthread_cond_wait(&tmp->car, &tmp->lock);

        tmp->buffer[tmp->next_out] = nullptr;
        --tmp->size;
        ++tmp->next_out;
        tmp->next_out %= tmp->capacity;
        ++tmp->cars_out;

        pthread_cond_signal(&tmp->space);
        pthread_mutex_unlock(&tmp->lock);
    }
    return nullptr;
}

void *Park::monitor(void *context) {
    Park *tmp = static_cast<Park *>(context);
    while (true) {
        usleep(PERIOD);
        pthread_mutex_lock(&tmp->lock);

        cout << "---------------------------" << endl;
        // 证明锁机制完成保护功能：cars_in - cars_out = size
        cout << "Delta = " << tmp->cars_in - tmp->cars_out - tmp->size << endl;
        cout << "Number of cars in carpark = " << tmp->size  << endl;
        for (int i = 0 ; i < tmp->capacity ; ++i)
            if (tmp->buffer[i] != nullptr)
                tmp->buffer[i]->print();

        cout << "---------------------------" << endl;
        pthread_mutex_unlock(&tmp->lock);
    }
    return nullptr;
}

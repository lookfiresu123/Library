#include "memory_pool.h"
#include "stack_alloc.h"
#include <vector>
#include <iostream>

// 插入元素个数
#define ELEMS 10000000

// 重复次数
#define REPS 100

using std::cin;
using std::cout;
using std::endl;

int main() {
    clock_t start;
    // 使用STL的默认分配器
    StackAlloc<int, allocator<int>> stack_default;
    start = clock();
    for (int i = 0 ; i < REPS ; ++i) {
        assert(stack_default.empty());
        for (int j = 0 ; j < ELEMS ; ++j)
            stack_default.push(j);
        for (int j = 0 ; j < ELEMS ; ++j)
            stack_default.pop();
    }
    cout << "Default Allocator Time: " << (((double)clock() - start) / CLOCKS_PER_SEC)<< endl;

    // 使用内存池
    StackAlloc<int, MemoryPool<int>> stack_pool;
    start = clock();
    for (int i = 0 ; i < REPS ; ++i) {
        assert(stack_pool.empty());
        for (int j = 0 ; j < ELEMS ; ++j)
            stack_pool.push(j);
        for (int j = 0 ; j < ELEMS ; ++j)
            stack_pool.pop();
    }
    cout << "MemoryPool Allocator Time: " << (((double)clock() - start) / CLOCKS_PER_SEC)<< endl;
    return 0;
}



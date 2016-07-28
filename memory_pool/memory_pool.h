#ifndef _H_MEMORY_POOL
#define _H_MEMORY_POOL

#include <climits>
#include <cstddef>
#include <utility>
#include <cstdint>

using std::forward;
using std::uintptr_t;

template <typename T, size_t BlockSize = 4096>
class MemoryPool {
public:
    typedef T *     pointer;
    template <typename U>
        struct rebind {
            typedef MemoryPool<U> other;
        };
    // 默认构造函数
    MemoryPool() noexcept : currentBlock_(nullptr), currentSlot_(nullptr),
        lastSlot_(nullptr), freeSlot_(nullptr) {}

    // 析构函数，即销毁一个现有的内存池
    ~MemoryPool() noexcept {
        slot_pointer_ cur = currentBlock_;
        while (cur != nullptr) {
            slot_pointer_ prev = cur->next;
            operator delete(reinterpret_cast<void *>(cur));
            cur = prev;
        }
    }

    // 同一时间只能分配一个对象，n和hint会被忽略
    pointer allocate(size_t n = 1, const T *hint = 0) {
        // 如果由空闲的对象槽，则直接将空闲区域交付出去
        if (freeSlot_ != nullptr) {
            pointer result = reinterpret_cast<pointer>(freeSlot_);
            freeSlot_ = freeSlot_->next;
            return result;
        } else {
            // 如果对象槽不够用，则重新分配一个新的内存区块
            if (currentSlot_ >= lastSlot_) {
                // 分配一个新的内存区块，并指向前一个内存区块
                data_pointer_ new_block = reinterpret_cast<data_pointer_>(operator new(BlockSize));
                reinterpret_cast<slot_pointer_>(new_block)->next = currentBlock_;
                currentBlock_ = reinterpret_cast<slot_pointer_>(new_block);

                // 填补整个区块来满足元素内存区域的对齐要求
                data_pointer_ body = new_block + sizeof(slot_pointer_);
                uintptr_t result = reinterpret_cast<uintptr_t>(body);
                size_t bodyPadding = (alignof(slot_type_) - result) % alignof(slot_type_);

                // 更新当前对象槽为新的内存区块的第一个对象槽
                currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
                // 更新最后一个对象槽为新的内存区块的最后一个对象槽
                lastSlot_ = reinterpret_cast<slot_pointer_>(new_block + BlockSize - sizeof(slot_type_) + 1);
            }
            // 返回第一个空闲（当前）的对象槽，并将更新对象槽
            return reinterpret_cast<pointer>(currentSlot_++);
        }
    }

    // 销毁指针p所指向的内存块
    void deallocate(pointer p, size_t n = 1) {
        if (p != nullptr) {
            reinterpret_cast<slot_pointer_>(p)->next = freeSlot_;   // 将p所指对象槽加入空闲对象槽
            freeSlot_ = reinterpret_cast<slot_pointer_>(p);         // 更新指向第一个空闲对象槽的指针
        }
    }

    // 构造内存池中的一个对象
    template <typename U, typename... Args>
    void construct(U *p, Args&&... args) {
        new (p) U(forward<Args>(args)...);
    }

    // 析构内存池中的一个对象
    template <typename U>
    void destroy(U *p) { p->~U(); }

private:
    // 内存池中用于存储对象的槽，要么存储对象，要么存储指针
    union Slot_ {
        T element;          // 对象槽中的对象
        Slot_ *next;        // 指向下一个对象槽的指针
    };

    typedef char *  data_pointer_;  // 数据指针
    typedef Slot_   slot_type_;     // 对象槽类型
    typedef Slot_ * slot_pointer_;  // 指向对象槽的指针

    slot_pointer_ currentBlock_;    // 指向当前内存区块（一个内存区块有多个对象槽）的指针
    slot_pointer_ currentSlot_;     // 指向当前内存区块中的当前对象槽的指针
    slot_pointer_ lastSlot_;        // 指向当前内存区块中的最后一个对象槽的指针
    slot_pointer_ freeSlot_;        // 指向当前内存区块中的第一个空闲对象槽的指针

    // 检查定义的内存池大小是否过小
    static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize is too small.");
};

#endif

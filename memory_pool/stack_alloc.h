#ifndef _H_STACK_ALLOC
#define _H_STACK_ALLOC 1

#include <vector>
#include <memory>
#include <cassert>
using std::allocator;

// 模板链表结点
template <typename T>
struct StackNode_ {
    T data;
    StackNode_ *prev;   // 由于栈是后进先出的，因此用prev代替next
};

// 模板链表栈
template <typename T, typename Alloc = allocator<T>>
class StackAlloc {
public:
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other allocator;

    // 默认构造函数
    StackAlloc() : head_(nullptr) {}
    // 默认析构函数
    ~StackAlloc() { clear(); }

    // 当栈中元素为空时，返回真
    bool empty() { return head_ == nullptr; }
    // 是否栈中所有的元素
    void clear() {
        Node *cur = head_;
        while (cur != nullptr) {
            Node *tmp = cur->prev;
            allocator_.destroy(cur);
            allocator_.deallocate(cur, 1);
            cur = tmp;
        }
        head_ = nullptr;
    }
    // 压栈
    void push(T element) {
        Node *new_node = allocator_.allocate(1);
        allocator_.construct(new_node, Node());
        new_node->data = element;
        new_node->prev = head_;
        head_ = new_node;
    }
    // 出栈
    T pop() {
        assert(!empty());
        T result = head_->data;
        Node *tmp = head_->prev;
        allocator_.destroy(head_);
        allocator_.deallocate(head_, 1);
        head_ = tmp;
        return result;
    }
    // 返回栈顶元素
    T top() { return head_->data; }

private:
    allocator allocator_;
    Node *head_;        // 栈顶
};


#endif

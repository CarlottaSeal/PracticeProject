// 实现一个固定容量、存 int 的环形缓冲区（循环队列）RingBuffer：
// RingBuffer(size_t capacity)：初始化容量为 capacity 的缓冲区
// bool push(int value)：在队尾加入元素；满了返回 false，不覆盖
// bool pop(int& out)：取出队头元素放进 out；空了返回 false
// int& front()：返回队头元素（调用前保证不为空）
// bool empty() const / bool full() const / size_t size() const
// 所有操作都必须是 O(1)，不能在 push/pop 时移动其他元素，构造之后不能再分配内存。
//
// 示例：
// RingBuffer rb(3);
// rb.push(1);    // true，[1]
// rb.push(2);    // true，[1, 2]
// rb.push(3);    // true，[1, 2, 3]，full() == true
// rb.push(4);    // false，满了
// rb.pop(x);     // true，x = 1，[2, 3]
// rb.push(4);    // true，[2, 3, 4]，写指针绕回了数组开头
// rb.size();     // 3
//
// 约束：
// 1 <= capacity <= 10⁶
//
// 追问：
// 1. 只用 head 和 tail 两个下标时，空和满都是 head == tail，怎么区分？有几种办法？
//    答：三种。(a) 另外记一个 count，就是这里的写法。(b) 浪费一个格子：(tail + 1) % cap == head 算满，head == tail 算空。
//        (c) head 和 tail 一直往上加、不回绕，取下标时再 % cap，size = tail - head，size == cap 算满。
//        (b) 和 (c) 不需要一个两边都要改的 count，这一点在 lock-free 的 SPSC 版本里很关键。
// 2. 如果容量是 2 的幂，取模可以换成什么？
//    答：i & (cap - 1)。按位与只要一条指令，% 要做整数除法，慢很多（几十个周期）。用户给的容量不是 2 的幂时，可以先用 nextPowerOfTwo 向上取整。
// 3. 改成满了就覆盖最旧的元素（比如只保留最近 N 帧的历史），哪些地方要改？
//    答：满了的时候 push 不再返回 false，而是照常写到 tail，然后 tail 和 head 都往前走一格，丢掉最旧的元素，count 保持等于 cap。
//        push 永远成功，也可以把被覆盖的那个值返回出去。日志、帧时间统计、游戏里的 rewind 历史都是这种用法。
// 4. 一个线程只 push、另一个线程只 pop（单生产者单消费者），能不能不加锁？head 和 tail 要用什么类型？
//    答：能，这就是 lock-free SPSC queue。producer 只写 tail，consumer 只写 head，各自只读对方的那个下标。
//        head 和 tail 必须是 std::atomic<size_t>，否则就是 data race（UB），编译器还可能把值缓存在寄存器里，或者调换读写顺序。
//        不能用共享的 count，因为两个线程都要改它，所以要换成上面的 (b) 或 (c)。
// 5. 上一问里，push 写完数据再更新 tail，用什么 memory_order？为什么？
//    答：producer 先写 data[tail]，再 tail.store(新值, memory_order_release)；consumer 先 tail.load(memory_order_acquire)，再读数据。
//        release 和 acquire 配对，保证 consumer 看到新的 tail 时，对应的数据一定已经写好了，写数据不会被重排到更新 tail 之后。
//        head 反过来同样配对。各自读自己的那个下标可以用 relaxed。用 seq_cst 也对，只是比需要的更强、更慢。
// 6. head 和 tail 放在同一条缓存行里会有什么问题？
//    答：false sharing。producer 写 tail、consumer 写 head，两个变量在逻辑上毫无关系，但在同一条 64 字节的 cache line 里，
//        每写一次都会让另一个核心的那份 cache line 失效（cache coherence 协议，比如 MESI），cache line 在两个核心之间来回传，性能会掉很多。
//        解决办法是用 alignas(64)（或者 std::hardware_destructive_interference_size）把它们放到不同的 cache line 上；还可以在本地缓存一份对方的下标，减少去读对方的次数。
#include <cstddef>
#include <vector>
using namespace std;

class RingBuffer
{
    vector<int> m_data;
    size_t m_count;
    size_t m_head;
    size_t m_tail;
public:
    explicit RingBuffer(size_t capacity):
        m_data(capacity),
        m_count(0),
        m_head(0),
        m_tail(0)
    {

    }

    bool push(int value)
    {
        if (full()) return false;

        m_data[m_tail] = value;
        m_tail = (m_tail + 1) % m_data.size();
        m_count++;
        return true;
    }

    bool pop(int& out)
    {
        if (empty()) return false;

        out = m_data[m_head];
        m_head = (m_head + 1) % m_data.size();
        m_count--;
        return true;
    }

    int& front()
    {
        return m_data[m_head];
    }

    bool empty() const
    {
        return m_count == 0;
    }

    bool full() const
    {
        return m_count == m_data.size();
    }

    size_t size() const
    {
        return m_count;
    }
};

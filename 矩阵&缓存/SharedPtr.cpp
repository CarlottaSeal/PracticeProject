// 不使用 <memory>，手写一个简化版 SharedPtr<T>（共享所有权、引用计数的智能指针）：
// explicit SharedPtr(T* p = nullptr)：接管 p，引用计数为 1（p 为空时不持有任何东西）
// 拷贝构造、拷贝赋值：共享同一个对象，引用计数 +1
// 移动构造、移动赋值：转移所有权，引用计数不变，原来的对象变成空
// ~SharedPtr()：引用计数 -1，减到 0 时释放对象和计数
// long use_count() const：当前有几个 SharedPtr 共享这个对象（空的返回 0）
// T* get() const / T& operator*() const / T* operator->() const
// void reset(T* p = nullptr)：放弃当前对象（计数 -1），改为接管 p
// 不要求线程安全，不要求实现 weak_ptr。
//
// 示例：
// SharedPtr<int> a(new int(5));   // a.use_count() == 1
// {
//     SharedPtr<int> b = a;       // use_count == 2
//     SharedPtr<int> c;
//     c = b;                      // use_count == 3
// }                               // b、c 析构，use_count == 1
// SharedPtr<int> d = std::move(a);// a 变空，d.use_count() == 1
// d.reset();                      // 计数减到 0，int 被释放
//
// 追问：
// 1. 引用计数为什么不能直接作为 SharedPtr 的成员变量？要放在哪里？
//    答：计数必须被所有共享同一个对象的 SharedPtr 一起看到、一起修改。如果是成员变量，每个副本都有自己的一份，彼此不同步。
//        所以要放在堆上单独分配的 control block 里，每个副本都持有指向它的指针。标准库的 control block 里还有 weak count、deleter 和 allocator。
// 2. 拷贝赋值里，先给新对象 +1 还是先给旧对象 -1？自己赋值给自己（a = a）时会怎样？
//    答：先给新的 +1，再给旧的 -1。这样即使没有自赋值检查也是安全的：计数从 1 变成 2，再变回 1。
//        如果先 -1，计数为 1 时会先减到 0 把对象删掉，之后再去 +1 就是在操作已经释放的内存（use-after-free）。
// 3. 多个线程同时拷贝同一个 SharedPtr，计数要怎么改？这样之后，它指向的对象是不是也线程安全了？
//    答：计数要用 std::atomic。+1 用 fetch_add(relaxed) 就够了；-1 用 fetch_sub(acq_rel)，保证其他线程之前对对象的写入，在 delete 之前都可见。
//        这样 control block 是线程安全的：不同线程各自持有不同的 SharedPtr 副本，同时拷贝或析构都没问题。但多个线程同时写同一个 SharedPtr 对象本身还是会 race，要用 C++20 的 std::atomic<std::shared_ptr>。
//        指向的对象不会因此变得线程安全，访问它还是要自己加 mutex。
// 4. 两个对象用 SharedPtr 互相指着，会发生什么？weak_ptr 是怎么解决的？
//    答：形成 reference cycle，两边的计数都永远降不到 0，内存泄漏。weak_ptr 是不拥有所有权的观察者，不增加 strong count。
//        把其中一个方向改成 weak_ptr（比如子节点指回父节点那一条）就能打破循环。需要访问时用 lock()，对象还活着就返回一个 shared_ptr，否则返回空。
// 5. 有了 weak_ptr 之后，控制块里要存几个计数？对象和控制块分别在什么时候释放？
//    答：两个：strong count（就是 use_count）和 weak count。strong 减到 0 时析构对象；strong 和 weak 都减到 0 时才释放 control block。
//        常见的实现是让所有 strong 合起来在 weak count 里再占 1。
// 6. make_shared 和 SharedPtr<T>(new T) 有什么区别？各有什么代价？
//    答：make_shared 只分配一次内存，对象和 control block 挨在一起，分配次数少、locality 好，也是 exception-safe 的；new T 的写法要分配两次。
//        make_shared 的代价：对象和 control block 是一块内存，只要还有 weak_ptr 在，即使对象已经析构了，这块内存也释放不了，对大对象不友好；它也不能指定自定义删除器。
// 7. 在类的成员函数里想拿到指向自己的 SharedPtr，为什么不能直接 SharedPtr<T>(this)？
//    答：这样会新建一个独立的 control block。同一个对象就被两个 control block 各自管理，两边计数分别减到 0 时各 delete 一次，就是 double free。
//        正确做法是继承 std::enable_shared_from_this<T>，调用 shared_from_this()。它内部存了一个指向原来 control block 的 weak_ptr。前提是这个对象已经被某个 shared_ptr 管理着。
#include <atomic>
#include <utility>

template <typename T>
class SharedPtr
{
    T* m_ptr;
    std::atomic<int>* m_count;
public:
    explicit SharedPtr(T* p = nullptr)
    {
        m_ptr = p;
        if (p)
        {
            m_count = new std::atomic<int>(1);
        }
        else
        {
            m_count = nullptr;
        }
    }

    SharedPtr(const SharedPtr& other)
    {
        m_ptr = other.m_ptr;
        m_count = other.m_count;
        if (m_count)
        {
            ++(*m_count);
        }
    }

    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this == &other) return *this; // 自赋值，直接返回
        if (other.m_count)
        {
            (*other.m_count) ++;
        }
        release();
        m_count = other.m_count;
        m_ptr = other.m_ptr;
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
    {
        m_count = other.m_count;
        m_ptr = other.m_ptr;
        other.m_count = nullptr;
        other.m_ptr = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept
    {
        if (this == &other) return *this; // 自赋值，直接返回
        release();
        m_count = other.m_count;
        m_ptr = other.m_ptr;
        other.m_count = nullptr;
        other.m_ptr = nullptr;
        return *this;
    }

    ~SharedPtr()
    {
        release();
    }

    long use_count() const
    {
        return m_count? m_count->load() : 0;
    }

    T* get() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    void reset(T* p = nullptr)
    {
        *this = SharedPtr(p);
        //*this = 临时对象 会调用移动赋值，移动赋值里会先 release() 自己原来的对象，再接管临时对象的 m_ptr 和 m_count。
    }
private:
    void release()
    {
        if (m_count && --(*m_count) == 0)
        {
            delete m_ptr;
            delete m_count;
        }
        m_ptr = nullptr;
        m_count = nullptr;
    }
};

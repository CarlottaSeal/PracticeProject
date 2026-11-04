// 不使用 <memory>，手写一个简化版 UniquePtr<T>（独占所有权的智能指针）：
// explicit UniquePtr(T* p = nullptr)：接管 p
// ~UniquePtr()：释放持有的对象
// 禁止拷贝构造和拷贝赋值
// 移动构造、移动赋值：转移所有权，原来的对象变成空
// T* get() const：返回裸指针，不转移所有权
// T* release()：放弃所有权，返回裸指针，自己变成空（不释放对象）
// void reset(T* p = nullptr)：释放当前对象，改为接管 p
// T& operator*() const / T* operator->() const
// explicit operator bool() const：是否持有对象
//
// 示例：
// UniquePtr<int> a(new int(5));
// UniquePtr<int> b = std::move(a);   // a.get() == nullptr，*b == 5
// UniquePtr<int> c = b;              // 编译错误：不能拷贝
// b.reset(new int(7));               // 原来的 5 被释放，*b == 7
// int* raw = b.release();            // b 变空，raw 需要手动 delete
// delete raw;
//
// 追问：
// 1. 为什么要禁止拷贝？不禁止会出什么问题？
//    答：它表达的是 exclusive ownership。允许拷贝的话，两个对象会持有同一个指针，析构时各 delete 一次，就是 double free（UB）。
//        所有权的转移必须显式地用 std::move 来写。
// 2. 移动构造和移动赋值为什么要标 noexcept？和 vector 扩容有什么关系？
//    答：vector 扩容搬元素时用的是 std::move_if_noexcept：移动构造是 noexcept 才用移动，否则退回拷贝，以保证 strong exception guarantee。
//        搬到一半抛异常的话，已经移走的元素没法恢复，拷贝则不会破坏原来的数据。UniquePtr 不能拷贝，所以 vector 还是会移动它，但标上 noexcept 本身就是如实声明：交换一个指针不可能抛异常，编译器也能据此优化。
// 3. 移动赋值里，自己赋值给自己（a = std::move(a)）会不会出问题？
//    答：有 this != &other 的检查就没事。没有检查的话：先 delete m_ptr，对象没了；接着 m_ptr = other.m_ptr，还是那个已经释放的指针；
//        最后 other.m_ptr = nullptr，因为是同一个对象，结果 a 变空。不会 double free，但对象丢了。
//        标准库写成 reset(other.release())：release 先把指针取出来并把自己置空，reset 再接管它，自赋值时也能保住对象。
// 4. reset 里先 delete 旧指针还是先换成新指针？p 和当前指针相同时会怎样？
//    答：先换再删：T* old = m_ptr; m_ptr = p; delete old;。这样如果 ~T() 里又访问到这个 UniquePtr，看到的已经是新的、一致的状态。
//        p == m_ptr 时，刚接管的对象会被立刻 delete，之后就是 dangling pointer，属于 UB。这是调用方的错误，标准库也不检查。
// 5. 要支持 UniquePtr<int[]>，哪里要改？
//    答：写一个 partial specialization template <typename T> class UniquePtr<T[]>：析构和 reset 用 delete[]，不用 delete；
//        提供 operator[]，去掉 * 和 ->。用 new[] 分配却用 delete 释放是 UB。
// 6. 加上自定义删除器之后，sizeof(UniquePtr) 会变吗？
//    答：看删除器有没有状态。stateless 的删除器（空类、没有捕获的 lambda）靠 Empty Base Optimization 或者 [[no_unique_address]] 不占空间，还是 8 字节。
//        函数指针做删除器就要多存一个指针，变成 16 字节。有状态的删除器还要再加上它自己的大小。
// 7. 为什么推荐用 make_unique，而不是直接 new？
//    答：一是 exception safety：C++17 之前，f(UniquePtr<A>(new A), g()) 的求值顺序可能是先 new A，再调 g()，g() 抛异常时 A 就泄漏了。
//        二是不用把类型写两遍，代码里也不出现裸的 new。shared_ptr 还有一个好处是 make_shared 只分配一次内存，unique_ptr 没有这个区别。
#include <utility>

template <typename T>
class UniquePtr
{
    T* m_ptr;
public:
    explicit UniquePtr(T* p = nullptr):m_ptr(p)
    {
    }

    ~UniquePtr()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept :m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (m_ptr != other.m_ptr)
        {
            delete m_ptr; //先释放自己当前对象
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    T* get() const
    {
        return m_ptr;
    }

    T* release()
    {
        T* p = m_ptr;
        m_ptr = nullptr;
        return p;
    }

    void reset(T* p = nullptr)
    {
        if (m_ptr != p)
        {
            T* old = m_ptr;
            m_ptr = p;
            delete old;
        }
    }

    T& operator*() const
    {
        return *m_ptr;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    explicit operator bool() const
    {
        return m_ptr != nullptr;
    }
};

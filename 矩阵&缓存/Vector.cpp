﻿// 不使用 <vector>，手写一个简化版动态数组 MyVector<T>：
// MyVector()：空数组，size 和 capacity 都是 0
// 拷贝构造、拷贝赋值：深拷贝
// 移动构造、移动赋值：接管对方的内存，对方变成空数组
// ~MyVector()：析构所有元素并释放内存
// void push_back(const T& value) / void push_back(T&& value)：在末尾加一个元素，容量不够时扩容
// void pop_back()：删掉最后一个元素（调用前保证不为空）
// T& operator[](size_t i) / const T& operator[](size_t i) const
// size_t size() const / size_t capacity() const / bool empty() const
// void reserve(size_t newCap)：把容量至少扩到 newCap，不改变 size；newCap 不大于当前容量时什么都不做
// T* begin() / T* end()：支持范围 for 循环
// push_back 的均摊复杂度必须是 O(1)。
//
// 示例：
// MyVector<int> v;      // size 0，capacity 0
// v.push_back(1);       // size 1
// v.push_back(2);
// v.push_back(3);       // size 3，capacity 至少是 3
// v[1] = 20;            // [1, 20, 3]
// v.pop_back();         // [1, 20]
// v.reserve(100);       // size 2，capacity 100
// MyVector<int> w = v;  // 深拷贝，改 w[0] 不影响 v
// for (int x : v) { }   // 依次取到 1、20
//
// 约束：
// T 不一定有默认构造函数
//
// 追问：
// 1. 每次扩容为什么是乘以一个倍数，而不是加一个固定值？为什么 push_back 均摊是 O(1)？
//    答：每次加固定值 c 的话，push n 次要扩容 n/c 次，总共要搬约 n²/(2c) 个元素，平均每次 push 是 O(n)。
//        按倍数 k 扩容的话，总搬运量不超过 n × (1 + 1/k + 1/k² + …) = n × k/(k-1)，是 O(n)，平均到每次 push 就是 amortized O(1)。
// 2. 扩容倍数取 2 和取 1.5 各有什么考虑？
//    答：×2 扩容次数更少，但最多会浪费一半的空间。而且之前释放掉的那些块加起来（1 + 2 + 4 + … = 2^k - 1）总比下一次要的 2^k 小，allocator 永远没法拿它们拼出新块来复用。
//        ×1.5 浪费的空间更少，倍数小于黄金分割比 1.618 时，旧块加起来最终能够凑出新块，有机会复用内存。libstdc++ 和 libc++ 用 2，MSVC 用 1.5。
// 3. T 没有默认构造函数时，为什么不能用 new T[capacity] 分配内存？应该怎么做？
//    答：new T[cap] 会对全部 cap 个位置调用默认构造函数。T 没有默认构造就编译不过；即使有，也白白构造了一堆没用的对象，size 和 capacity 的语义也被混在一起了。
//        正确做法是把"分配内存"和"构造对象"分开：用 ::operator new(cap * sizeof(T))（或者 std::allocator<T>::allocate）分配原始内存，
//        用 placement new：new (ptr) T(args) 在指定位置构造，用 ptr->~T() 显式析构，最后用 ::operator delete 释放。
// 4. 扩容时搬运旧元素，用拷贝还是移动？为什么 T 的移动构造要标 noexcept 才会被用上？
//    答：能移动就移动，具体由 std::move_if_noexcept 决定：移动构造是 noexcept（或者 T 不能拷贝）时才移动，否则拷贝。
//        这是为了 strong exception guarantee：移动到一半抛异常的话，旧缓冲区里已经被移走的元素没法恢复；拷贝则不会动旧数据，可以完整回滚。所以 noexcept 直接影响扩容的性能。
// 5. pop_back 时只把 size 减 1 够不够？元素的析构函数谁来调？
//    答：对 int 够了，但对管理资源的 T（比如 std::string）不够，会泄漏。要手动调用 m_data[--m_size].~T()，标准库里是通过 allocator_traits::destroy 调用的。
// 6. 扩容之后，之前拿到的指针、引用、迭代器还能用吗？
//    答：都不能用了。扩容会把所有元素搬到新内存，旧的指针、引用、迭代器全部 invalidated。不扩容的话，push_back 只会让 end() 失效。
//        常见 bug：一边拿着某个元素的引用，一边往同一个 vector 里 push_back。
// 7. v.push_back(v[0]) 在刚好需要扩容时会出什么问题？
//    答：参数 value 是对旧缓冲区里 v[0] 的引用。如果扩容时先释放了旧缓冲区，再用 value 构造新元素，读到的就是已经释放的内存（use-after-free）。
//        解决办法：先在新缓冲区里构造好新元素，再搬旧元素、释放旧缓冲区；或者先把 value 复制到一个临时变量里。标准库的实现都处理了这种情况。
// 8. emplace_back 和 push_back 有什么区别？
//    答：push_back 接收一个已经构造好的对象，再把它拷贝或移动进去。emplace_back(args...) 用 variadic template 加 perfect forwarding 把参数原样转发，
//        直接用 placement new 在 vector 的内存上构造，省掉了一个临时对象和一次移动，还能调用 explicit 构造函数。如果传进去的本来就是一个现成的对象，两者没有区别。
#include <cstddef>
#include <new>
#include <utility>

template <typename T>
class MyVector
{
    T* m_data = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

public:
    MyVector()
    {
    }

    MyVector(const MyVector& other)
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }

    MyVector& operator=(const MyVector& other)
    {
        if (this == &other) return *this;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        return *this;
    }

    MyVector(MyVector&& other) noexcept
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept
    {
        if (this == &other) return *this;
        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    ~MyVector()
    {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    void push_back(const T& value)
    {
        grow();
        m_data[m_size++] = value;
    }

    void push_back(T&& value)
    {
        grow();
        m_data[m_size++] = value;
    }

    void pop_back()
    {
        delete m_data[m_size - 1];
    }

    T& operator[](size_t i)
    {
        return m_data[i];
    }

    const T& operator[](size_t i) const
    {
        return m_data[i];
    }

    size_t size() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_capacity;
    }

    bool empty() const
    {
        return m_size == 0;
    }

    void reserve(size_t newCap)
    {
        m_capacity = newCap;
        grow();
    }

    T* begin()
    {
        return m_data[0];
    }

    T* end()
    {
        return m_data[m_size - 1];
    }

private:
    void grow()
    {
        if ()
    }
};


// ===================== 简化版：不用模板，只存 int =====================
// 和上面的接口一样，只是元素固定是 int，所以可以直接用 new int[] / delete[]，不用处理构造和析构。
//
// 示例：
// IntVector v;          // size 0，capacity 0
// v.push_back(1);       // size 1，capacity 1
// v.push_back(2);       // size 2，capacity 2
// v.push_back(3);       // size 3，capacity 4
// v[1] = 20;            // [1, 20, 3]
// v.pop_back();         // [1, 20]
// v.reserve(100);       // size 2，capacity 100
// IntVector w = v;      // 深拷贝，改 w[0] 不影响 v
// for (int x : v) { }   // 依次取到 1、20
//
// 追问：
// 1. push_back 的参数为什么用 int 而不是 const int&？如果改成 const int&，v.push_back(v[0]) 在刚好扩容时会出什么问题？
//    答：int 只有 4 字节，按值传和传引用一样便宜，而且按值传拿到的是一份独立的副本，和 vector 的缓冲区没有关系，所以 push_back(v[0]) 一定安全。
//        改成 const int& 的话，value 引用的是旧缓冲区里的 v[0]：grow() 里的 reserve 会先 delete[] 旧缓冲区，之后 m_data[m_size++] = value 读到的就是已经释放的内存（use-after-free）。
//        对很大的 T，一般还是传 const T&，但实现里就必须处理这种 aliasing。
class IntVector
{
    int* m_data = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

public:
    IntVector()
    {
    }

    IntVector(const IntVector& other)
    {
        if (other.m_capacity > 0)
        {
            m_data = new int[other.m_capacity];
            for (size_t i = 0; i < other.m_size; ++i)
                m_data[i] = other.m_data[i];
        }
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }

    IntVector& operator=(const IntVector& other)
    {
        if (this == &other) return *this;
        int* newData = nullptr;
        if (other.m_capacity > 0)
        {
            newData = new int[other.m_capacity]; // 先分配新的，new 失败时原对象不受影响
            for (size_t i = 0; i < other.m_size; ++i)
                newData[i] = other.m_data[i];
        }
        delete[] m_data;
        m_data = newData;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        return *this;
    }

    IntVector(IntVector&& other) noexcept
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    IntVector& operator=(IntVector&& other) noexcept
    {
        if (this == &other) return *this;
        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    ~IntVector()
    {
        delete[] m_data;
    }

    void push_back(int value)
    {
        grow();
        m_data[m_size++] = value;
    }

    void pop_back()
    {
        --m_size; // int 不需要析构，size 减 1 就行
    }

    int& operator[](size_t i)
    {
        return m_data[i];
    }

    const int& operator[](size_t i) const
    {
        return m_data[i];
    }

    size_t size() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_capacity;
    }

    bool empty() const
    {
        return m_size == 0;
    }

    void reserve(size_t newCap)
    {
        if (newCap <= m_capacity) return;
        int* newData = new int[newCap];
        for (size_t i = 0; i < m_size; ++i) // 只搬 size 个，后面的空位不用管
            newData[i] = m_data[i];
        delete[] m_data;
        m_data = newData;
        m_capacity = newCap;
    }

    int* begin()
    {
        return m_data;
    }

    int* end()
    {
        return m_data + m_size; // 指向最后一个元素的下一个位置
    }

private:
    void grow()
    {
        if (m_size < m_capacity) return;               // 还有空位，不用扩
        reserve(m_capacity == 0 ? 1 : m_capacity * 2); // 空的先给 1，之后每次翻倍
    }
};

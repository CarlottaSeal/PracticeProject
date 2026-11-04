// 不使用任何库函数，手写下面五个 C 字符串函数（字符串以 '\0' 结尾）：
// size_t myStrlen(const char* s)：返回字符串长度，不含 '\0'
// char* myStrcpy(char* dst, const char* src)：把 src（含 '\0'）复制到 dst，返回 dst
// int myStrcmp(const char* a, const char* b)：a < b 返回负数，相等返回 0，a > b 返回正数
// char* myStrcat(char* dst, const char* src)：把 src 接到 dst 末尾，返回 dst
// const char* myStrstr(const char* haystack, const char* needle)：返回 needle 在 haystack 中第一次出现的位置，找不到返回 nullptr
//
// 示例：
// myStrlen("hello") → 5
// myStrlen("")      → 0
//
// myStrcmp("abc", "abd") → 负数
// myStrcmp("abc", "abc") → 0
// myStrcmp("abc", "ab")  → 正数
//
// char buf[16] = "foo";
// myStrcat(buf, "bar");  // buf 变成 "foobar"
//
// myStrstr("hello", "ll") → 指向 "hello" 下标 2 的位置
// myStrstr("hello", "")   → 指向 "hello" 开头
// myStrstr("abc", "d")    → nullptr
//
// 约束：
// 传入的指针都不为空
// dst 的空间足够大
//
// 追问：
// 1. strcpy 为什么不安全？strncpy 又有什么坑？
//    答：strcpy 不检查 dst 有多大，src 太长就会 buffer overflow，这是经典的安全漏洞来源。
//        strncpy 的坑：src 长度 ≥ n 时，它不会写结尾的 '\0'，得到的字符串没有结尾；src 比较短时，又会把剩下的空间全部填 0，浪费。更安全的选择是 snprintf 或者 std::string。
// 2. strcmp 比较字符时，为什么要转成 unsigned char？
//    答：char 是有符号还是无符号由实现决定（implementation-defined）。如果是 signed，大于 127 的字符（比如 0xE9）会变成负数，排序就错了。标准规定按 unsigned char 比较。
// 3. 在循环里反复调用 strcat 拼接字符串，复杂度是多少？怎么改？
//    答：每次 strcat 都要从 dst 开头扫到 '\0'，拼 n 次总共是 O(n²)，俗称 "Schlemiel the Painter" 算法。
//        改法：自己记住结尾的位置，直接从结尾往后写；或者先算好总长度一次分配；或者用 std::string 的 +=，它记录了 size，而且是 amortized O(1)。
// 4. 你写的 strstr 复杂度是多少？有没有更快的做法？
//    答：暴力匹配最坏是 O(n·m)，比如在 "aaaa…a" 里找 "aa…ab"。更快的有 KMP，用 prefix function 做到 O(n + m)；还有 Boyer-Moore（平均情况 sublinear）和 Rabin-Karp（rolling hash）。
// 5. std::string 和 C 字符串比，有什么区别？什么是 SSO（短字符串优化）？
//    答：std::string 自己保存 size，取长度是 O(1)，内容里也可以有 '\0'；它用 RAII 自己管理内存，会自动扩容，还有带边界检查的 at()。C 字符串只是一个以 '\0' 结尾的 char 数组。
//        SSO（Small String Optimization）：短字符串直接存在对象自己内部的小缓冲区里，不用 heap allocation。MSVC 和 libstdc++ 能存 15 个字符，libc++ 能存 22 个。
#include <cstddef>

size_t myStrlen(const char* s)
{
    const char* p = s;
    while (*p) ++p;
    return p - s;
}

char* myStrcpy(char* dst, const char* src)
{
    
}

int myStrcmp(const char* a, const char* b)
{

}

char* myStrcat(char* dst, const char* src)
{

}

const char* myStrstr(const char* haystack, const char* needle)
{

}

// 实现一个简化版字符串类 MyString，内部自己管理一块以 '\0' 结尾的堆内存（可以用上面自己写的函数）：
// MyString()：空字符串
// MyString(const char* s)：从 C 字符串构造
// 拷贝构造、拷贝赋值：深拷贝，两个对象各自拥有一份内存
// 移动构造、移动赋值：直接接管对方的内存，对方变成空字符串
// ~MyString()：释放内存
// size_t size() const：长度，不含 '\0'
// const char* c_str() const：返回以 '\0' 结尾的 C 字符串
// char& operator[](size_t i) / const char& operator[](size_t i) const
// MyString& operator+=(const MyString& other)：把 other 接到末尾
// MyString operator+(const MyString& a, const MyString& b)：返回拼接后的新字符串
// bool operator==(const MyString& a, const MyString& b)：内容相同返回 true
// std::ostream& operator<<(std::ostream& os, const MyString& s)：输出内容
//
// 示例：
// MyString a("hello");      // a.size() == 5
// MyString b = a;           // 深拷贝，b[0] = 'H' 之后 a 仍然是 "hello"
// MyString c = std::move(a);// c == "hello"，a 变成空字符串，a.size() == 0
// c += MyString(" world");  // c == "hello world"
// MyString d = b + c;       // d == "Hellohello world"
// std::cout << d;           // 输出 Hellohello world
// MyString e;               // e.size() == 0，e.c_str() 返回 ""，不是 nullptr
//
// 追问：
// 1. 不自己写拷贝构造，用编译器默认生成的会出什么问题？
//    答：默认生成的是逐成员的 shallow copy，两个对象的 m_data 指向同一块内存。析构时这块内存会被 delete 两次（double free，UB），改其中一个另一个也会跟着变。
// 2. 拷贝赋值里自己赋值给自己（a = a）会怎样？copy-and-swap 是怎么同时解决这个问题和异常安全的？
//    答：如果先 delete 自己的内存再从 other 复制，自赋值时 other 就是自己，读到的是已经释放的内存（use-after-free）。
//        copy-and-swap：operator=(MyString other) 参数按值传，先拷贝一份；然后 swap(*this, other)，再 return *this，旧数据跟着 other 一起析构。
//        拷贝发生在修改 *this 之前，拷贝失败时 *this 不受影响（strong exception guarantee）；自赋值也天然安全，只是多拷贝了一次。
// 3. 这五个函数（析构、拷贝构造、拷贝赋值、移动构造、移动赋值）为什么要一起考虑？什么是 Rule of Five / Rule of Zero？
//    答：它们管理的是同一份资源的所有权。需要自定义其中一个，通常说明类在直接管理资源，其他几个也都要自定义，这就是 Rule of Five。
//        另外，自己声明了析构或拷贝函数，编译器就不会再自动生成移动函数，用到移动的地方会退回成拷贝。
//        Rule of Zero：不直接管理裸资源，成员都用 std::string、std::vector、std::unique_ptr 这类会自己管理的类型，五个函数一个都不用写，编译器生成的就是对的。
// 4. 移动之后的对象，为什么要让它变成合法的空字符串，而不是留一个空指针？
//    答：被移动后的对象之后还会被析构，还可能被重新赋值甚至继续使用。标准要求它处于 "valid but unspecified state"，类的 invariant 必须仍然成立。
//        这个类允许 m_data 为空，但 c_str() 会返回 ""、size() 返回 0，所以对外看起来它就是一个合法的空字符串。
// 5. 反复调用 += 时，每次都重新分配刚好够用的内存，复杂度是多少？加一个 capacity 怎么改进？
//    答：每次 += 都要把整个字符串复制一遍，拼 n 次总共是 O(n²)。加上 m_capacity，空间不够时按倍数扩容（比如 ×2），和 vector 一样，均摊下来每次是 amortized O(1)，只有 size 超过 capacity 才重新分配。
// 6. operator+ 为什么写成非成员函数？
//    答：为了左右两边对称。非成员函数的两个参数都能做 implicit conversion，所以 "hi" + s 也能用，const char* 会通过非 explicit 的构造函数转成 MyString。
//        写成成员函数的话，左边必须本来就是 MyString。而且它用 += 就能实现，不需要访问私有成员。
// 7. 怎么给这个类加上 SSO（短字符串优化）？
//    答：用一个 union { char* heap; char local[16]; } 再加上 size。长度 ≤ 15 时存在 local 里，否则存在 heap 上。还要有办法区分当前是哪种模式，比如用一个标志位，或者看指针是不是指向 local。
//        拷贝和移动都要分两种情况处理：短字符串移动时不能直接偷指针，只能复制字节。c_str() 根据模式返回 local 或 heap。
#include <cstring> // 修正：strlen、memcpy 需要这个头文件
#include <ostream>
#include <utility>

class MyString
{
    char* m_data;
    size_t m_size;
public:
    MyString()
    {
        m_data = nullptr;
        m_size = 0;
    }

    MyString(const char* s)
    {
        m_size = strlen(s); // 修正：原来多加了 1，m_size 不含 '\0'
        m_data = new char[m_size + 1];
        std::memcpy(m_data, s, m_size+1);
    }

    MyString(const MyString& other)
    {
        m_data = new char[other.m_size + 1];
        std::memcpy(m_data, other.c_str(), other.m_size + 1); // 修正：other 为空时 m_data 是 nullptr，要通过 c_str() 读
        m_size = other.m_size;
    }

    MyString& operator=(const MyString& other)
    {
        if (this == &other) return *this;
        char* newData = new char[other.m_size + 1]; // 先分配新的，new 失败时原对象不受影响
        std::memcpy(newData, other.c_str(), other.m_size + 1);
        delete[] m_data;                            // 再释放旧的
        m_data = newData;
        m_size = other.m_size;
        return *this;
    }

    MyString(MyString&& other) noexcept
    {
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
    }

    MyString& operator=(MyString&& other) noexcept
    {
        if (this == &other) return *this;
        delete[] m_data; // 和移动构造的区别：要先释放自己原来的内存
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
        return *this;
    }

    ~MyString()
    {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
    }

    size_t size() const
    {
        return m_size;
    }

    const char* c_str() const
    {
        return m_data ? m_data : ""; // 修正：空字符串时 m_data 是 nullptr，要返回 ""
    }

    char& operator[](size_t i)
    {
        return m_data[i];
    }

    const char& operator[](size_t i) const
    {
        return m_data[i];
    }

    MyString& operator+=(const MyString& other)
    {
        char* newData = new char[m_size + other.m_size + 1];
        std::memcpy(newData, this->c_str(), m_size);
        std::memcpy(newData + m_size, other.c_str(), other.m_size + 1); // 带上 '\0'
        delete[] m_data; // a += a 也没问题：delete 之前数据已经复制进 newData 了
        m_data = newData;
        m_size += other.m_size;
        return *this;
    }
};

MyString operator+(const MyString& a, const MyString& b)
{
    MyString result(a); // 修改：原来分配了两次内存，改成复用 +=
    result += b;
    return result;
}

bool operator==(const MyString& a, const MyString& b)
{
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) // 修改：int 改成 size_t，去掉有符号/无符号比较的警告
    {
        if (a[i] != b[i]) return false;        
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const MyString& s)
{
    return os << s.c_str(); // 返回 os，才能连着写 cout << a << b
}

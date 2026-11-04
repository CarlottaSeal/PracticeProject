// 不使用任何库函数，手写下面三个内存操作函数：
// void* myMemcpy(void* dst, const void* src, size_t n)：从 src 复制 n 个字节到 dst，可以假设两段内存不重叠
// void* myMemmove(void* dst, const void* src, size_t n)：同上，但两段内存可能重叠，结果必须正确
// void* myMemset(void* dst, int value, size_t n)：把 dst 开始的 n 个字节都设成 value
// 三个函数都返回 dst。
//
// 示例：
// char buf[] = "abcdef";
// myMemmove(buf + 2, buf, 4);   // buf 变成 "ababcd"（dst 在 src 后面且重叠）
//
// char buf[] = "abcdef";
// myMemmove(buf, buf + 2, 4);   // buf 变成 "cdefef"（dst 在 src 前面且重叠）
//
// char buf[8];
// myMemset(buf, 'x', 3);        // buf 前 3 个字节是 'x'
//
// 约束：
// n 可以为 0
// dst 和 src 不为空
//
// 追问：
// 1. memcpy 遇到重叠内存会出什么问题？为什么标准说是未定义行为？
//    答：从前往后复制时，前面写进 dst 的字节可能覆盖了 src 里还没读到的部分，复制结果就错了。
//        标准规定重叠是 UB，是为了让实现可以自由优化：可以按任意方向复制、一次搬很多字节、用 SIMD，编译器也可以假设两个指针不重叠（restrict）。
// 2. memmove 什么时候要从后往前复制？什么时候从前往后？
//    答：dst 在 src 后面并且有重叠（src < dst < src + n）时，要从后往前复制，这样每个字节被覆盖之前都已经读出来了；dst 在 src 前面时从前往后复制。
// 3. 为什么参数是 void*，实现里要转成什么类型？
//    答：void* 是通用指针，任何对象指针都能隐式转成它，所以一个函数能处理任何类型。但 void* 不能解引用，也不能做指针运算，
//        所以要转成 unsigned char*，按字节访问。char 类型是 strict aliasing 规则的例外，可以合法地访问任何对象的字节。
// 4. 想按 8 字节一次复制来加速，要先处理什么问题？
//    答：alignment。没对齐的 8 字节读写在有的架构上很慢甚至直接报错，在 C++ 里也是 UB。做法是先逐字节复制，直到地址对齐到 8，再按 8 字节复制，最后补完剩下的尾巴。
//        如果 src 和 dst 对 8 取模的余数不同，两边没法同时对齐，只能用 unaligned load 或者移位拼接。另外还要注意 strict aliasing。
// 5. int arr[4]; memset(arr, 1, sizeof(arr)); 之后 arr[0] 是多少？为什么？
//    答：16843009，也就是 0x01010101。memset 是按字节设置的，每个字节都被设成 0x01，一个 int 有 4 个字节。只有设成 0 或者 -1（每个字节都是 0xFF）时，结果才和直觉一致。
// 6. 为什么返回 dst？
//    答：C 标准库的约定，方便连着写，比如把返回值直接传给下一个函数。
#include <cstddef>
#include <cstdint>

void* myMemcpy(void* dst, const void* src, size_t n)
{
    const unsigned char* s = (const unsigned char*)src;
    unsigned char* d = (unsigned char*)dst;
    for (size_t i = 0; i < n; ++i)
    {
        d[i] = s[i];
    }
    return dst;
}

void* myMemmove(void* dst, const void* src, size_t n)
{
    const unsigned char* s = (const unsigned char*)src;
    unsigned char* d = (unsigned char*)dst;
    if (d == s) return dst;
    if ( d < s )
    {
        for (size_t i = 0; i < n; ++i)
        {
            d[i] = s[i];
        }
        
    }
    else
    {
        for (size_t i = n; i > 0; --i)
        {
            d[i - 1] = s[i - 1]; // 修正：原来是 d[i] = s[i]，会越界访问下标 n，且漏掉下标 0
        }
    }
    return dst;
}

void* myMemset(void* dst, int value, size_t n)
{
    unsigned char* d = (unsigned char*)dst;
    unsigned char byte = (unsigned char)value;
    uint32_t word = byte;
    word |= (word << 8);
    word |= (word << 16);
    size_t i = 0;
    while ( i<n && reinterpret_cast<uintptr_t>(d+i)%4 != 0 )
    {
        d[i] = byte;
        ++i;
    }
    for (; i + 4 <= n; i += 4)
    {
        *reinterpret_cast<uint32_t*>(d + i) = word;
    }
    for (; i < n; ++i)
    {
        d[i] = byte;
    }
    return dst;
}

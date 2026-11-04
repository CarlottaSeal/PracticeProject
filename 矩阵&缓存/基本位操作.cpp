// 用位运算（& | ^ ~ << >>）实现下面这些函数。除非题目注明，否则不要用循环一位一位地判断。
// 1. countBits(x)：x 的二进制里有几个 1（可以用循环，但每轮要直接消掉一个 1）
// 2. isPowerOfTwo(x)：x 是不是 2 的幂
// 3. lowestBit(x)：只保留 x 最低位的那个 1
// 4. setBit / clearBit / toggleBit / testBit：对第 k 位置 1、清 0、取反、判断是否为 1
// 5. reverseBits(x)：把 32 位无符号整数的二进制位整个倒过来
// 6. singleNumber(nums)：数组里除了一个数只出现一次，其他数都出现两次，找出它
// 7. alignUp(x, a)：把 x 向上对齐到 a 的整数倍，a 是 2 的幂
// 8. nextPowerOfTwo(x)：大于等于 x 的最小的 2 的幂
// 9. pack / unpack：把格子坐标 x（0~15）、y（0~15）、z（0~127）打包成一个整数：低 4 位存 x，接着 4 位存 y，再往上存 z；以及反过来解包
// 10. morton2D(x, y)：把 x 和 y 的二进制位交错排列，结果第 2i 位是 x 的第 i 位，第 2i+1 位是 y 的第 i 位
// 11. prevPowerOfTwo(x)：小于等于 x 的最大的 2 的幂，x 为 0 时返回 0
//
// 示例：
// countBits(11)        → 3        （1011）
// isPowerOfTwo(16)     → true
// isPowerOfTwo(0)      → false
// isPowerOfTwo(6)      → false
// lowestBit(12)        → 4        （1100 → 0100）
// setBit(5, 1)         → 7        （101 → 111）
// clearBit(7, 0)       → 6
// toggleBit(5, 2)      → 1
// testBit(5, 2)        → true
// reverseBits(1)       → 2147483648（0x80000000）
// singleNumber([4,1,2,1,2]) → 4
// alignUp(13, 8)       → 16
// alignUp(16, 8)       → 16
// nextPowerOfTwo(17)   → 32
// nextPowerOfTwo(16)   → 16
// nextPowerOfTwo(1)    → 1
// pack(3, 5, 10)       → 2643
// morton2D(5, 3)       → 27       （x=101, y=011 → 011011）
// prevPowerOfTwo(100)  → 64
// prevPowerOfTwo(64)   → 64
// prevPowerOfTwo(0)    → 0
//
// 约束：
// 除 singleNumber 外，输入都是 uint32_t
// nextPowerOfTwo 的输入 1 <= x <= 2^31
// morton2D 的 x、y 都小于 2^16
//
// 追问：
// 1. x & (x - 1) 做了什么？countBits 和 isPowerOfTwo 都能用它，分别怎么用？
//    答：消掉最低位的那个 1。x - 1 会把最低位的 1 变成 0、把它右边的 0 全变成 1，再和 x 做 & 就只少了那一个 1。
//        countBits：反复做直到变成 0，做了几次就有几个 1（Kernighan's algorithm）。isPowerOfTwo：2 的幂只有一个 1，消掉以后就是 0，还要排除 x == 0。
// 2. x & -x 为什么能取出最低位的 1？和补码有什么关系？
//    答：two's complement 下，-x = ~x + 1。~x 把每一位都翻转，原来最低位那个 1 右边的 0 都变成了 1；再加 1 会一路进位，直到那个位置重新变回 1。
//        结果是：这个位置以上的位和 x 相反，以下的位都是 0，所以和 x 做 & 只剩下最低位的那个 1。
// 3. 对有符号整数右移，左边补的是什么？对负数左移有什么问题？
//    答：C++20 之前，负数右移是 implementation-defined，主流编译器做的是 arithmetic shift，左边补符号位 1；C++20 正式规定为 arithmetic shift。
//        负数左移、或者把 1 移进符号位造成溢出，在 C++20 之前是 UB。所以做位运算尽量用 unsigned 类型。
// 4. 1 << 31 写成 int 有什么问题？应该怎么写？
//    答：字面量 1 是有符号的 int，左移 31 位会移进符号位，C++20 之前是 UB。实际得到的是 INT_MIN，再转成 64 位时还会 sign extension 成 0xFFFFFFFF80000000。
//        应该写 1u << 31；需要 32 位以上时写 1ull << k。另外不管什么类型，移位数 ≥ 位宽都是 UB。
// 5. alignUp 里为什么要求 a 是 2 的幂？不是的话要怎么写？
//    答：只有 a 是 2 的幂时，a - 1 才是"低几位全是 1"的掩码，~(a - 1) 才能把低位清零。a 不是 2 的幂时用整数除法：((x + a - 1) / a) * a，结果一样，只是慢一些。
// 6. morton2D 怎么扩展到三维？为什么 BVH 构建、纹理存储会用 Morton 序？
//    答：三维是把 x、y、z 三个数的位交错排列，x 的第 i 位放到第 3i 位，y 放到 3i+1，z 放到 3i+2。每个坐标取 10 位，拼成 30 位的编码。
//        Morton 序也叫 Z-order curve，好处是保持 spatial locality：空间里相邻的点，编码也相近。
//        LBVH（Karras 2012）按图元中心点的 Morton code 排序，再按最高的不同位来切分，适合在 GPU 上并行构建。
//        纹理用 swizzled/tiled layout 存储，二维上相邻的 texel 会落在同一条 cache line 里，bilinear 采样或者斜着采样时 cache 命中率更高。
// 7. 怎么判断机器是大端还是小端？
//    答：uint32_t x = 1; 然后看 *(unsigned char*)&x 是不是 1。是 1 说明最低字节存在最低地址，就是 little-endian。
//        x86 和绝大多数 ARM 都是 little-endian，网络字节序是 big-endian（所以有 htonl 这类函数）。C++20 里可以直接用 std::endian。
#include <cstdint>
#include <vector>
using namespace std;

uint32_t countBits(uint32_t x)
{
    uint32_t count = 0;
    while (x!= 0)
    {
        x &= (x - 1); //消掉最低位的 1
        count++;
    }
    return count;
}

bool isPowerOfTwo(uint32_t x)
{
    return x>0 && (x & (x - 1)) == 0;
}

uint32_t lowestBit(uint32_t x)
{
    return x & (~x + 1);   // ~x + 1 就是 -x
}

uint32_t setBit(uint32_t x, int k)
{
    return x | (1u << k);
}

uint32_t clearBit(uint32_t x, int k)
{
    return x & ~(1u << k);
}

uint32_t toggleBit(uint32_t x, int k)
{
    return x ^ (1u << k);
}

bool testBit(uint32_t x, int k)
{
    return (x >> k) & 1u;
}

uint32_t reverseBits(uint32_t x)
{
    uint32_t result = 0;
    for (int i  = 0; i < 32; ++i)
    {
        result <<= 1;
        result |= (x & 1u);
        x >>= 1;
    }
    return result;
}

int singleNumber(const vector<int>& nums)
{
    int a = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        a ^= nums[i];
    }
    return a;
}

uint32_t alignUp(uint32_t x, uint32_t a)
{
    return (x + a -1) & ~(a - 1);
}

uint32_t nextPowerOfTwo(uint32_t x)  //大于等于 x 的最小 2 的幂
{
    x --;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
//     - 中间五行把最高位的 1 往右边"涂满"：10000 变成 11111。
// - 再加 1 就进位成 100000。
// - 先减 1 是为了处理 x 本身就是 2 的幂的情况：比如 16，不减的话会得到 32。
}

uint32_t prevPowerOfTwo(uint32_t x)  //小于等于 x 的最大 2 的幂
{
    for (int i = 31; i >= 0; --i)    // i 用 int：倒着减到 0 时，无符号数的 i >= 0 永远成立
    {
        if ((x >> i) & 1u)           // 从高往低找，第一个 1 就是最高位
            return 1u << i;
    }
    return 0;
// 不用循环的写法：和 nextPowerOfTwo 一样先把最高位以下涂满（不用先减 1），再 return x - (x >> 1);
}

uint32_t pack(uint32_t x, uint32_t y, uint32_t z)
{
    return x | (y << 4) | (z << 8);
}

void unpack(uint32_t packed, uint32_t& x, uint32_t& y, uint32_t& z)
{
    for (int i = 0; i < 4; ++i)
    {
        x |= ((packed >> i) & 1u) << i;
        y |= ((packed >> (i + 4)) & 1u) << i;
        //z |= ((packed >> (i + 8)) & 1u) << i;
    }
    for (int i = 0; i < 7; ++i)             // 2. z 有 7 位，要单独循环 7 次
        z |= ((packed >> (i + 8)) & 1u) << i;
}

uint32_t morton2D(uint32_t x, uint32_t y)  //交错排列
{
    uint32_t r = 0;
    for (int i = 0; i < 16; ++i)
    {
        r |= ((x >> i) & 1u) << (2 * i);
        r |= ((y >> i) & 1u) << (2 * i + 1);
    }
    return r;
}

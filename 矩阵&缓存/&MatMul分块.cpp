// 给定两个 n × n 的浮点矩阵 A 和 B，计算 C = A × B。
// 三个矩阵都用一维数组按行存储：元素 (i, j) 存在下标 i * n + j。
// 依次实现下面四个版本，结果必须完全相同：
// 1. matmulNaive：朴素三重循环，顺序 i-j-k
// 2. matmulIKJ：把循环顺序改成 i-k-j
// 3. matmulTiled：分块计算，块边长为 tile；n 不一定是 tile 的整数倍
// 4. GPU 版：每个线程算 C 的一个元素，用 shared memory 分块（写在文件末尾的注释区，伪代码即可）
// 每个函数内部负责把 C 清零。
//
// 示例：
// 输入：n = 2
// A = [1 2      B = [5 6
//      3 4]          7 8]
// 输出：
// C = [19 22
//      43 50]
//
// 约束：
// 1 <= n <= 2048
// 1 <= tile <= n
//
// 追问：
// 1. 四个版本的时间复杂度分别是多少？哪些版本只改变了常数？
//    答：四个版本的总运算量都是 O(n³)，改变的只是常数，也就是 cache miss 和访存次数。GPU 版的总工作量也是 O(n³)，只是分给很多线程并行做。
//        渐近意义上更快的是 Strassen，O(n^2.81)，但常数大、数值稳定性差，实际很少用。
// 2. 为什么 i-k-j 比 i-j-k 快？内层循环分别在访问哪些地址？
//    答：数组是 row-major 存储。i-j-k 的内层循环走 k：A[i*n+k] 是连续的，但 B[k*n+j] 每次跳 n 个元素（stride n），几乎每次都 cache miss。
//        i-k-j 的内层循环走 j：C[i*n+j] 和 B[k*n+j] 都是 unit stride 连续访问，A[i*n+k] 在内层循环里不变，可以放在寄存器里。连续访问还能被编译器 SIMD 向量化。
// 3. 先把 B 转置再用 i-j-k，为什么也能变快？
//    答：转置以后，B 的一列变成了 Bᵀ 的一行，内层循环变成两行之间的点积，两边都是 unit stride。转置本身是 O(n²)，相对 O(n³) 可以忽略。
// 4. tile 取多大合适？由什么决定？
//    答：由要放进的那一级 cache 决定。A、B、C 三个 tile 要能同时放进去：3 × T² × 4 字节 ≤ cache 大小。比如 L1 是 32 KB 时，T 大约 32 到 48。
//        一般取 2 的幂，并且是 cache line 和 SIMD 宽度的整数倍，最后靠实测调参。GPU 上则受每个 block 的 shared memory 容量和 occupancy 限制，常用 16 或 32。
// 5. GPU 版里 __syncthreads() 要放在哪里？少了会出什么问题？
//    答：每处理一个 tile 要同步两次。第一次在 tile 加载进 shared memory 之后、计算之前，保证所有线程都写完了（RAW）；
//        第二次在计算之后、加载下一个 tile 之前，保证没有线程还在读旧 tile（WAR）。少了第一次会读到没写好的数据，少了第二次会出现 data race，结果错误而且每次运行都可能不一样。
// 6. n 不是 TILE 的整数倍时，GPU 版怎么处理？
//    答：grid 维度用 ceil(n / TILE)。加载 tile 时下标越界的位置填 0（zero padding），这样点积结果不受影响；最后写 C 的时候也要判断 row < n && col < n。
// 7. 什么是合并访存？你的 GPU 版里 A 和 B 的读取是不是合并的？
//    答：coalesced access：同一个 warp 里相邻的线程访问相邻的地址，硬件就能把它们合并成少数几次 32 或 128 字节的内存事务。
//        threadIdx.x 对应列时，读 A 的 tile 是 A[row*n + t*TILE + tx]，读 B 的 tile 是 B[(t*TILE + ty)*n + col]，相邻 tx 读的都是相邻地址，所以都是合并的，写 C 也是。
//        反过来如果把 tx 对应到行，就会变成 strided 访问，无法合并。
// 8. 什么情况下把数据放进 shared memory 反而没有收益？
//    答：数据复用率低的时候，比如每个元素只读一两次，或者访问不规则（像 BVH 遍历，每条射线走的路径都不一样）；
//        以及数据本来就在 L1/L2 里命中率很高的时候。这时搬进 shared memory 的拷贝开销、__syncthreads 的开销，以及占用 shared memory 导致 occupancy 下降，会抵消掉收益。
// 9. 改成 M×K 乘 K×N 的长方形矩阵，哪些地方要改？
//    答：循环上界分别改成 i < M、j < N、k < K；下标改成 A[i*K + k]、B[k*N + j]、C[i*N + j]。
//        GPU 版的 grid 改成 ceil(N / TILE) × ceil(M / TILE)，tile 循环次数是 ceil(K / TILE)，三个维度分别做边界检查。
#include <vector>
using namespace std;

void matmulNaive(const vector<float>& A, const vector<float>& B, vector<float>& C, int n)
{
//C[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + ... + A[i][n-1]*B[n-1][j]
    C.assign(n * n, 0);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            float sum = 0.f;    //在寄存器
            for (int k = 0; k < n; ++k)
            {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] += sum;
        }
    }
}

void matmulNaiveMNK(const vector<float>& A, const vector<float>& B, vector<float>& C, int M, int N, int K)
{
    C.assign(M * N, 0.0f);
    for (int i = 0; i < M; ++i)            // C 的行，最多 M 行
    {
        for (int j = 0; j < N; ++j)        // C 的列，最多 N 列
        {
            float sum = 0.0f;
            for (int k = 0; k < K; ++k)    // 点积长度 K
                sum += A[i * K + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
    }
}

void matmulIKJ(const vector<float>& A, const vector<float>& B, vector<float>& C, int n)
{
    C.assign(n * n, 0);
    for (int i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            float a_ik = A[i * n + k]; // A[i][k]，在寄存器
            for (int j = 0; j < n; ++j)
            {
                C[i * n + j] += a_ik * B[k * n + j];
            }
        }
    }
}

void matmulIKJMNK(const vector<float>& A, const vector<float>& B, vector<float>& C, int M, int N, int K)
{
    C.assign(M * N, 0.f);
    for (int i = 0; i < M; i++)
    {
        for (int k = 0; k < K; k ++)
        {
            float a_ik = A[i * K + k];
            for (int j = 0; j < N; ++j)
            {
                C[i*N + j] += a_ik * B[k * N + j];
            }
        }
    }
}

void matmulTiled(const vector<float>& A, const vector<float>& B, vector<float>& C, int n, int tile)
{
    C.assign(n*n, 0.f);
    for (int ii = 0; ii < n; ii += tile)
    {
        for (int kk = 0; kk < n; kk += tile)
        {
            for (int jj = 0; jj < n; jj += tile)
            {
                //最后一块可能不够 tile 行，如果还用 ii + tile 就会超出矩阵。所以要和 n 取较小的那个。
                int iEnd = min(ii + tile, n);//iEnd 是当前这一块的 i 走到哪里为止，是块内循环的上界，不包含它本身。
                int kEnd = min(kk + tile, n);
                int jEnd = min(jj + tile, n);
                for (int i = ii; i < iEnd; ++i)
                {
                    for (int k = kk; k < kEnd; ++k)
                    {
                        float a_ik = A[i * n + k]; 
                        for (int j = jj; j < jEnd; ++j)
                        {
                            C[i * n + j] += a_ik * B[k * n + j];
                        }
                    }
                }
            }
        }
    }
}

// GPU 版（伪代码，不参与编译）
// #define TILE 16
// 启动配置：blockDim = (TILE, TILE)，gridDim = (ceil(n / TILE), ceil(n / TILE))
//
//#ifdef __CUDACC__
#define TILE 16
__global__ void matmulTiledGPU(const float* A, const float* B, float* C, int n)
{
    __shared__ float As[TILE][TILE];
    __shared__ float Bs[TILE][TILE];

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int row = blockIdx.y * TILE + ty;
    int col = blockIdx.x * TILE + tx;

    float sum = 0.f;

    for ( int t = 0; t < (n + TILE - 1 )/ TILE; t++)   // 沿 k 方向的第 t 块，向上取整
    {
        int aCol = t*TILE + tx;   // 我要搬 A 的第 row 行第 aCol 列
        int bRow = t*TILE + ty;   // 我要搬 B 的第 bRow 行第 col 列

        As[ty][tx] = (row < n && aCol < n) ? A[row * n + aCol] : 0.f;
        Bs[ty][tx] = (col < n && bRow < n) ? B[bRow * n + col] : 0.f;

        __syncthreads();  //1. 等所有人都搬完，才能开始读
        for (int k = 0; k < TILE; ++k)
        {
            sum += As[ty][k] * Bs[k][tx];
        }
        __syncthreads();  //2. 等所有人都算完，才能进下一轮覆盖
    }
    if (row < n && col < n)
    {
        C[row * n + col] = sum;
    }
}
//#endif
// launch：
// dim3 block(TILE, TILE);
// dim3 grid((n + TILE - 1) / TILE, (n + TILE - 1) / TILE);
// matmulTiledGPU<<<grid, block>>>(dA, dB, dC, n);

// 1. row 和 col：blockIdx 选的是 C 的哪一块，threadIdx 选的是块内的哪个格子。x 对应列，y 对应行。
// 2. 搬数据：每个线程正好搬 A 的一个元素和 B 的一个元素，256 个线程合起来就是两个 16×16 的块。
//    - 搬 A 时，同一个 warp 里相邻的 tx 读的是 A[row*n + t*TILE + tx]，地址连续，可以合并成一次显存访问（coalescing，合并访问）。
//    - 搬 B 时，相邻的 tx 读 B[bRow*n + col]，col 也是连续的，同样能合并。
// 3. 为什么越界补 0：n 不是 TILE 的整数倍时，最后一块有一部分在矩阵外面。补 0 以后，这些位置参与乘加也只是加 0，不影响结果，块内循环还能固定走 TILE 次，不用再写 min。
// 4. 最后写回也要判断：row 或 col 超出 n 的线程也参与了搬数据，但它们没有对应的 C 格子，不能写。
//
// 两个 __syncthreads() 为什么都不能少
// - 第一个：防止读到别人还没写的数据。线程 A 要读 As[ty][k]，那个位置可能是线程 B 负责搬的。不等的话可能读到旧值。这是"先写后读"（RAW，read-after-write）的问题。
// - 第二个：防止别人还在读，自己就把下一轮的数据写进去。一个快的线程算完了，进入下一轮，往 As 里写新数据，这时慢的线程可能还在读上一轮的值。这是"先读后写"（WAR，write-after-read）的问题。
//
// 数据复用的效果
// 不分块的话，每个线程自己从显存读 A 的一行和 B 的一列，各 n 个数。分块后，每个元素从显存读进 shared memory 一次，就会被 block 里的 TILE 个线程用到，显存读取量降到原来的大约 1/TILE。TILE = 16 时，显存流量降到 1/16。
// As[ty][k] * Bs[k][tx] 和 CPU 版的对应关系：As 是 A 的一块，ty 是块内行号、k 是块内列号；Bs 是 B 的一块，k 是块内行号、tx 是块内列号。这就是 naive 版里 A[i][k] * B[k][j] 换到块内的坐标上。
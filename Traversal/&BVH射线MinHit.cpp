#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>
using namespace std;

constexpr float EPS = 1e-6f;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator*(float t) const { return {x*t, y*t, z*t}; }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vec3 cross(const Vec3& o) const {
        return {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x};
    }
};
struct Ray {
    Vec3 origin, dir;
    Vec3 invDir;  // 1/dir，预计算，slab 方法要用
};

struct AABB {
    Vec3 minP, maxP;
};

struct Triangle {
    Vec3 v0, v1, v2;
};

struct HitResult {
    float t = FLT_MAX;  // 命中距离
    bool hit = false;
};

struct BVHNode {
    AABB box;
    int left = -1, right = -1;      // 子节点索引，-1 表示无
    int triStart = -1, triCount = 0; // 叶子节点的三角形范围
    bool isLeaf() const { return triCount > 0; }
};

// Slab方法；返回是否相交，tMin 是进入距离（用来排序和剪枝）
bool intersectAABB(const Ray& ray, const AABB& box, float& tMin, float tMax)
{
    // 对 x, y, z 三组平行平面分别求进出距离，取交集
    float t0 = 0.f;
    float t1 = tMax;
    for (int i = 0; i < 3; i++)
    {
        float minVal = (&box.minP.x)[i];
        float maxVal = (&box.maxP.x)[i];

        float inv = (&ray.invDir.x)[i];
        float tNear = (minVal - (&ray.origin.x)[i]) * inv;
        float tFar = (maxVal - (&ray.origin.x)[i]) * inv;

        if (tNear > tFar)
        {
            swap(tNear, tFar);
        }
        t0 = max(t0, tNear);  // 起点取最大
        t1 = min(t1, tFar);    // 终点取最小

        if (t0>t1) return false;  //任何一组不相交都不行
    }
    tMin = t0;
    return true;
}

bool intersectTriangle(const Ray& ray, const Triangle& tri, float& t)
{
    Vec3 e1 = tri.v1 - tri.v0;
    Vec3 e2 = tri.v2 - tri.v0;
    Vec3 h = ray.dir.cross(e2);
    float a = e1.dot(h);
    
    if (fabs(a) < EPS) return false; //平行
    
    float f = 1.f/a;
    Vec3 s = ray.origin - tri.v0;
    float u = f * s.dot(h);
    if (u < 0.f || u > 1.f) return false;
    
    Vec3 q = s.cross(e1);
    float v = f * ray.dir.dot(q);
    if (v < 0.f || u + v > 1.f) return false;
    
    t = f * e2.dot(q);
    return t > EPS;    //明中距离要为正
}

HitResult queryBVH(const Ray& ray,
                   const vector<BVHNode>& nodes,
                   const vector<Triangle>& triangles)
{
    HitResult result;
    
    int stack[64];
    int top = 0;
    stack[top] = 0;
    top ++;
    
    while (top > 0)
    {
        int idx = stack[top - 1];
        top --;
        const BVHNode& node = nodes[idx];
        
        // 1. 射线和当前节点的 AABB 相交测试
        float tMin;
        if (!intersectAABB(ray, node.box, tMin, FLT_MAX)) //如果没命中，跳过
            continue;
        
        if (tMin > result.t) //如果命中了，但是距离大于当前最近距离，跳过
            continue;
        
        if (node.isLeaf()) //是叶子就遍历内部
        {
            for (int i = 0; i < node.triCount; i++)
            {
                float t = 0;
                if (intersectTriangle(ray, triangles[node.triStart + i], t))
                {
                    if (t < result.t)
                    {
                        result.t = t;
                        result.hit = true;
                    }
                }
            }
            continue; //叶子了，这个子树遍历完了，因此continue
        }
        //不是叶子，继续看左右子树
        float tLeft, tRight;
        bool hitLeft = intersectAABB(ray, nodes[node.left].box, tLeft, result.t);
        bool hitRight = intersectAABB(ray, nodes[node.right].box, tRight, result.t);
        if (hitLeft && hitRight)//两个都hit了
        {
            if (tLeft < tRight)  //比较距离
            {
                stack[top] = node.right; //远的先压
                top ++;
                stack[top] = node.left; //近的后压 ∴先出栈
                top ++;
            }
            else
            {
                stack[top] = node.left;
                top ++;
                stack[top] = node.right;
                top ++;
            }
        }
        else if (hitLeft)
        {
            stack[top] = node.left; 
            top ++;
        }
        else if (hitRight)
        {
            stack[top] = node.right;
            top ++;
        }
        // 都不相交就什么都不压，整棵子树被跳过
    }
    
    return result;
}
#include <accelerators/a3BVH.h>
#include <core/a3Ray.h>
#include <shapes/a3Shape.h>
#include <core/log/a3Log.h>
#include <core/a3Record.h>

//  TreeNode
a3BVHTreeNode::a3BVHTreeNode() :leftChild(NULL), rightChild(NULL)
{

}

void a3BVHTreeNode::initLeaf(a3Shape* _shape, unsigned int num, const a3AABB& b)
{
    primitive = _shape;
    bounds = b;
}

void a3BVHTreeNode::initInterior(unsigned int axis, a3BVHTreeNode *left, a3BVHTreeNode* right)
{
    leftChild = left;
    rightChild = right;

    // 包裹两子树的包围盒
    bounds = a3AABB::calUnion(left->bounds, right->bounds);

    splitAxis = axis;
    primitive = NULL;
}

// 依托印用于图元对象集合索引 不做实体primitive存储
class a3BVHPrimitive
{
public:
    a3BVHPrimitive() {}
    a3BVHPrimitive(a3Shape* shape, const a3AABB& b) :bounds(b), shape(shape)
    {
        centroid = 0.5f * b.min + 0.5f * b.max;
    }

    // 图元指针
    a3Shape* shape;

    // 几何中心
    t3Vector3f centroid;

    // 当前图元包围盒
    a3AABB bounds;
};


struct a3ComparePoints
{
    a3ComparePoints(int d):dimension(d) {}
    
    int dimension;
    bool operator()(const a3BVHPrimitive &a,
                    const a3BVHPrimitive &b) const
    {
        return a.centroid[dimension] < b.centroid[dimension];
    }
};

//  线性表形式BVH结点
class a3BVHLinearNode
{
public:
    // 包裹图元 / 两子树包围盒
    a3AABB bounds;

    union
    {
        // 叶子结点所需图元索引
        unsigned int primitiveIndex;
        // 内部结点所需的右子树索引(左子树紧跟根节点线性存放)
        unsigned int rightChildIndex;
    };

    // 叶子结点中存放的图元一般不超过255
    unsigned char primitiveNum;

    // 划分平面轴 x:0 y:1 z:2
    unsigned char axis;

    // 确保当前结点长度为32位以内存对齐
    unsigned char pad[2];
};

static inline bool a3RayBoxIntersect(const a3AABB &bounds, const a3Ray &ray, const float minT, const float maxT,
                              const t3Vector3f &invDir, const unsigned int dirIsNeg[3])
{
    // Check for ray intersection against $x$ and $y$ slabs
    float tmin = (bounds[dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    float tmax = (bounds[1 - dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    float tymin = (bounds[dirIsNeg[1]].y - ray.origin.y) * invDir.y;
    float tymax = (bounds[1 - dirIsNeg[1]].y - ray.origin.y) * invDir.y;
    if((tmin > tymax) || (tymin > tmax))
        return false;
    if(tymin > tmin) tmin = tymin;
    if(tymax < tmax) tmax = tymax;

    // Check for ray intersection against $z$ slab
    float tzmin = (bounds[dirIsNeg[2]].z - ray.origin.z) * invDir.z;
    float tzmax = (bounds[1 - dirIsNeg[2]].z - ray.origin.z) * invDir.z;
    if((tmin > tzmax) || (tzmin > tmax))
        return false;
    if(tzmin > tmin)
        tmin = tzmin;
    if(tzmax < tmax)
        tmax = tzmax;
    return (tmin < maxT) && (tmax > minT);
}

a3BVH::a3BVH():root(NULL)
{
    
}
 
a3BVH::~a3BVH()
{
    if(root)
        treeDelete(root);
}

// 转Shape为PrimitiveInfo
//      保证线性空间大小与Shapes一致
// 
// treeRoot = 递归构造(totalNode)
//  
// root = 分配线性空间(totalNode)
//
// root = linearBuild(treeRoot)
void a3BVH::init()
{
    int primitiveNum = primitives.size();

    if(primitiveNum <= 0)
    {
        a3Log::warning("a3BVH::init() 场景中shape对象为空，无需构建Accelerator\n");
        return;
    }

    std::vector<a3BVHPrimitive> bvhPrimitives;
    // 保证线性空间大小与Shapes一致
    //bvhPrimitives.reserve(primitives.size());
    for(int i = 0; i < primitives.size(); i++)
        bvhPrimitives.push_back(a3BVHPrimitive(primitives[i], primitives[i]->aabb));

    // 递归构造
    unsigned int nodeNum = 0;
    a3Log::success("BVH Tree Start Building\n");
    a3BVHTreeNode* treeRoot = treeBuild(0, primitives.size(), &nodeNum, bvhPrimitives);
    a3Log::success("BVH Tree Finished Building\n");

    root = treeRoot;

    // 二叉树中有结点2n-1个(n=图元个数)
    //root = new a3BVHLinearNode[nodeNum];
    // 扁平化树状为线性表
    //linearBuild(treeRoot, 0);
}

// 递归构造 start end primitives
//
//      dimension = 分割轴选择(aabb.maxExtent)     
// 
//      mid = 分割平面选择(equal / middle / SAH)
// 
//      totalNode ++       
//
//      if leaf
//          init leaf
//      else
//          node = new (buildLeft(start, mid), buildRight(mid, end))
//  
//      return node
a3BVHTreeNode* a3BVH::treeBuild(unsigned int start, unsigned int end, unsigned int* nodeNum,
                                std::vector<a3BVHPrimitive>& bvhPrimitives)
{
    // 当前结点根节点
    a3BVHTreeNode* root = new a3BVHTreeNode();

    unsigned int mid = (start + end) * 0.5f ;

    (*nodeNum)++;

    a3AABB maxBound;
    for(int i = start; i < end; i++)
        maxBound = a3AABB::calUnion(maxBound, bvhPrimitives[i].bounds);
    
    int range = end - start;

    // 叶子结点
    if(range == 1)
    {
        root->initLeaf(bvhPrimitives[start].shape, range, maxBound);
    }
    else
    {
        a3AABB centroidAABB;
        for(int i = start; i < end; ++i)
            centroidAABB = a3AABB::calUnion(centroidAABB, bvhPrimitives[i].centroid);

        // 分割轴选择
        int dimension = centroidAABB.maxExtentIndex();

        // SAH

        // Middel

        // Equal Counts
        mid = (start + end) / 2;
        std::nth_element(&bvhPrimitives[start], &bvhPrimitives[mid],
                         &bvhPrimitives[end - 1] + 1, a3ComparePoints(dimension));

        // 内部结点
        root->initInterior(dimension,
                           treeBuild(start, mid, nodeNum, bvhPrimitives),
                           treeBuild(mid, end, nodeNum, bvhPrimitives));
    }

    return root;
}

// 深度优先遍历
// 
// while 
//      root = linearBVHPrimitive[offset]
//      
//      offset++
// 
//      if 叶子结点
//          初始化叶子结点信息(root) 
//
//      else 内部结点
//
//          初始化内部结点信息(root) 
//
//          扁平化左孩子
//
//          index = 扁平化右孩子(获取右孩子在数组中的索引)
//
// return offset
// --!这里并不会改变传入的root,但因为需要访问其内部成员因此无法设置为const
unsigned int a3BVH::linearBuild(a3BVHTreeNode* treeRoot, unsigned int* offset)
{


    return 0;
}

// 遍历
bool a3BVH::intersect(const a3Ray& ray, a3IntersectRecord* intersection) const
{
    if(!root) return false;

    t3Vector3f inverseDirection(1.f / ray.direction.x, 
                                1.f / ray.direction.y, 
                                1.f / ray.direction.z);
    unsigned int dirIsNeg[3] = {inverseDirection.x < 0,
                                inverseDirection.y < 0, 
                                inverseDirection.z < 0};

    float minT = A3_INFINITY;
    float u = 0, v = 0;
    float vtu = 0, vtv = 0;
    a3Shape* shape = NULL;

    if(intersect(ray, root, &minT, &u, &v, &vtu, &vtv, inverseDirection, dirIsNeg, &shape))
    {
        intersection->t = minT;

        // 接触点初始化
        intersection->p = (ray) (minT);

        intersection->shape = shape;

        // 求交测试若遇到三角形那么[u, v]将被赋值
        intersection->u = u;
        intersection->v = v;

        // 若有纹理存在则纹理坐标将被赋值
        intersection->vtu = vtu;
        intersection->vtv = vtv;

        return true;
    }
    else
    {
        // 用于valid判断
        intersection->t = A3_INFINITY;

        return false;
    }
}

bool a3BVH::intersect(const a3Ray& ray) const
{
    if(!root) return false;

    t3Vector3f inverseDirection(1.f / ray.direction.x,
                                1.f / ray.direction.y,
                                1.f / ray.direction.z);
    unsigned int dirIsNeg[3] = {inverseDirection.x < 0,
        inverseDirection.y < 0,
        inverseDirection.z < 0};

    float minT = A3_INFINITY;
    float u = 0, v = 0;
    float vtu = 0, vtv = 0;
    a3Shape* shape = NULL;

    if(intersect(ray, root, &minT, &u, &v, &vtu, &vtv, inverseDirection, dirIsNeg))
        return true;
    else
        return false;
}

bool a3BVH::intersect(const a3Ray& ray, a3BVHTreeNode* node, float* minT, 
                      float* _u, float* _v, float* _vtu, float* _vtv,
                      const t3Vector3f &invDir, const unsigned int dirIsNeg[3],
                      a3Shape** shape) const
{
    if(a3RayBoxIntersect(node->bounds, ray, A3_EPSILON_FLOAT, *minT, invDir, dirIsNeg))
    {
        // 叶子结点
        if(node->primitive)
        {
            float t = A3_INFINITY;
            float u = 0, v = 0;
            float vtu = 0, vtv = 0;

            // 实际Ray Primitive相交测试
            //if(node->primitive->intersect(ray, &t, &u, &v, &vtu, &vtv) && t > A3_TOLERANCE_FLOAT && t < *minT)
            if(node->primitive->intersect(ray, &t, &u, &v) && t > A3_EPSILON_FLOAT && t < *minT)
            {
                *minT = t;
                *shape = node->primitive;
                *_u = u;
                *_v = v;
                *_vtu = vtu;
                *_vtv = vtv;

                return true;
            }
        }
        else
        {
            bool bHaveIntersection = false;
            // 判断左右子树遍历顺序
            if(dirIsNeg[node->splitAxis])
            {
                // 先遍历左子树
                bHaveIntersection |= intersect(ray, node->leftChild, minT, _u, _v, _vtu, _vtv, invDir, dirIsNeg, shape);
                bHaveIntersection |= intersect(ray, node->rightChild, minT, _u, _v, _vtu, _vtv, invDir, dirIsNeg, shape);
            }
            else
            {
                // 先遍历右子树
                bHaveIntersection |= intersect(ray, node->rightChild, minT, _u, _v, _vtu, _vtv, invDir, dirIsNeg, shape);
                bHaveIntersection |= intersect(ray, node->leftChild, minT, _u, _v, _vtu, _vtv, invDir, dirIsNeg, shape);
            }

            return bHaveIntersection;
        }
    }

    return false;
}

bool a3BVH::intersect(const a3Ray& ray, a3BVHTreeNode* node, float* minT, 
                      float* _u, float* _v, float* vtu, float* vtv,
                      const t3Vector3f &invDir, const unsigned int dirIsNeg[3]) const
{
    if(a3RayBoxIntersect(node->bounds, ray, A3_EPSILON_FLOAT, *minT, invDir, dirIsNeg))
    {
        // 叶子结点
        if(node->primitive)
        {
            float t = A3_INFINITY;
            float u = 0, v = 0;
            float vtu = 0, vtv = 0;

            // 实际Ray Primitive相交测试
            //if(node->primitive->intersect(ray, &t, &u, &v, &vtu, &vtv) && t > A3_TOLERANCE_FLOAT && t < *minT)
            if(node->primitive->intersect(ray, &t, &u, &v) && t > A3_EPSILON_FLOAT && t < *minT)
                return true;
        }
        else
        {
            bool bHaveIntersection = false;
            // 判断左右子树遍历顺序
            if(dirIsNeg[node->splitAxis])
            {
                // 先遍历左子树
                bHaveIntersection |= intersect(ray, node->leftChild, minT, _u, _v, vtu, vtv, invDir, dirIsNeg);
                bHaveIntersection |= intersect(ray, node->rightChild, minT, _u, _v, vtu, vtv, invDir, dirIsNeg);
            }
            else
            {
                // 先遍历右子树
                bHaveIntersection |= intersect(ray, node->rightChild, minT, _u, _v, vtu, vtv, invDir, dirIsNeg);
                bHaveIntersection |= intersect(ray, node->leftChild, minT, _u, _v, vtu, vtv, invDir, dirIsNeg);
            }

            return bHaveIntersection;
        }
    }
     
    return false;
}

void a3BVH::treeDelete(a3BVHTreeNode* root)
{
    if(root->leftChild)
        treeDelete(root->leftChild);

    if(root->rightChild)
        treeDelete(root->rightChild);

    A3_SAFE_DELETE(root);
}

#ifndef A3_BVH_H
#define A3_BVH_H

#include <core/a3AABB.h>
#include <accelerators/a3PrimitiveSet.h>

class a3BVHLinearNode;
//class a3BVHTreeNode;
class a3BVHPrimitive;

// 树状BVH结点
class a3BVHTreeNode
{
public:
    a3BVHTreeNode() :leftChild(NULL), rightChild(NULL) {}

    // 当前结点作为叶子结点
    // 叶子结点限制只能存放一个图元
    void initLeaf(a3Shape* _shape, unsigned int num, const a3AABB& b)
    {
        primitive = _shape;
        bounds = b;
    }

    // 当前结点作为内部结点
    void initInterior(unsigned int axis, a3BVHTreeNode *left, a3BVHTreeNode* right)
    {
        leftChild = left;
        rightChild = right;

        // 包裹两子树的包围盒
        bounds = a3AABB::calUnion(left->bounds, right->bounds);

        splitAxis = axis;
        primitive = NULL;
    }

    // 包裹左右子树的包围盒
    a3AABB bounds;

    // BVH树也是二叉树
    a3BVHTreeNode *leftChild, *rightChild;

    // 划分平面轴 x:0 y:1 z:2
    unsigned int splitAxis;

    // 图元指针
    a3Shape* primitive;
};

class a3BVH : public a3PrimitiveSet
{
public:
    // 叶子结点中最多存放图元个数(0, 255)
    // 暂不启用自定义个数 默认为1
    a3BVH();

    ~a3BVH();

    // objects可后续添加
    void init();

    virtual bool intersect(const a3Ray& ray, a3Intersection* intersection) const;

    virtual bool intersect(const a3Ray& ray) const;

//private:
    // 返回根节点
    a3BVHTreeNode* treeBuild(unsigned int start, unsigned int end, unsigned int* nodeNum,
                             std::vector<a3BVHPrimitive>& bvhPrimitives);

    // 转树形BVH为线性表
    unsigned int linearBuild(a3BVHTreeNode* node, unsigned int* offset);

    // 暂不启用线性表形式BVH
    a3BVHTreeNode* root;
};

#endif

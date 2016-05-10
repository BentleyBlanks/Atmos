#ifndef A3_BVH_H
#define A3_BVH_H

#include <core/a3AABB.h>
#include <accelerators/a3PrimitiveSet.h>

class a3BVHLinearNode;
//class a3BVHTreeNode;
class a3BVHPrimitive;

// ��״BVH���
class a3BVHTreeNode
{
public:
    a3BVHTreeNode();

    // ��ǰ�����ΪҶ�ӽ��
    // Ҷ�ӽ������ֻ�ܴ��һ��ͼԪ
    void initLeaf(a3Shape* _shape, unsigned int num, const a3AABB& b);

    // ��ǰ�����Ϊ�ڲ����
    void initInterior(unsigned int axis, a3BVHTreeNode *left, a3BVHTreeNode* right);

    // �������������İ�Χ��
    a3AABB bounds;

    // BVH��Ҳ�Ƕ�����
    a3BVHTreeNode *leftChild, *rightChild;

    // ����ƽ���� x:0 y:1 z:2
    unsigned int splitAxis;

    // ͼԪָ��
    a3Shape* primitive;
};

class a3BVH : public a3PrimitiveSet
{
public:
    // Ҷ�ӽ���������ͼԪ����(0, 255)
    // �ݲ������Զ������ Ĭ��Ϊ1
    a3BVH();

    ~a3BVH();

    // objects�ɺ������
    void init();

    virtual bool intersect(const a3Ray& ray, a3Intersection* intersection) const;

    virtual bool intersect(const a3Ray& ray) const;

    // �ݲ��������Ա���ʽBVH
    a3BVHTreeNode* root;

private:
    // ���ظ��ڵ�
    a3BVHTreeNode* treeBuild(unsigned int start, unsigned int end, unsigned int* nodeNum,
                             std::vector<a3BVHPrimitive>& bvhPrimitives);

    // ת����BVHΪ���Ա�
    unsigned int linearBuild(a3BVHTreeNode* node, unsigned int* offset);

    // �ڲ��ݹ��󽻱���
    // interset->intersection + bool
    bool intersect(const a3Ray& ray, a3BVHTreeNode* node, float* minT,
                   const t3Vector3f &invDir, const unsigned int dirIsNeg[3],
                   a3Shape** shape) const;

    // interset->bool
    bool intersect(const a3Ray& ray, a3BVHTreeNode* node, float* minT,
                   const t3Vector3f &invDir, const unsigned int dirIsNeg[3]) const;
};

#endif

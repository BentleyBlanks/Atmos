#ifndef A3_SHAPE_H
#define A3_SHAPE_H

#include <core/a3Spectrum.h>
#include <t3Math/core/t3Vector3.h>
#include <core/a3Settings.h>
#include <core/a3AABB.h>
#include <lights/a3AreaLight.h>

class a3Ray;
class a3BSDF;
class a3LightSample;

class a3Shape
{
public:
	a3Shape();

    a3Shape(std::string name);
    
    // [u, v]用于三角形插值计算当前相交点的重心坐标系位置; [vtu, vtv]用于相交点的纹理坐标计算
    virtual bool intersect(const a3Ray& ray, float* t, float* u, float* v, float* vtu, float* vtv) const;

    virtual t3Vector3f getNormal(const t3Vector3f& hitPoint, float u, float v) const;

    virtual void print() const;

    bool isEmitter() const;

    // inline
    const a3BSDF* getBSDF() const;

    a3BSDF* getBSDF();

    bool hasBSDF() const;

    void setBSDF(a3BSDF* bsdf);

    // 计算当前形状表面积
    virtual float area() const;

    float pdf(const t3Vector3f& p, const t3Vector3f& wi);

    // 给定采样区间获取采样点
    virtual t3Vector3f sample(const a3LightSample& sample) const;

    virtual a3AreaLight* getAreaLight() const;

	// 自发光系数
	t3Vector3f emission;

    // 折射率
    float refractiveIndex;

	// BSDF类型
    a3BSDF* bsdf;

    bool bIsEmitter;

    // 包围盒
    a3AABB aabb;

    // 是否使用单一面片法线
    bool bUseFaceNormal;

    bool bCalTextureCoordinate;

    std::string name;

    // 释放时机与其余light生命周期一致 shape释放时对此不做操作
    a3AreaLight* areaLight;
};

#endif
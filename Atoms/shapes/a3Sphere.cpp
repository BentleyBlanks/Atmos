#include <shapes/a3Sphere.h>
#include <core/a3Ray.h>

a3Sphere::a3Sphere(const t3Vector3f& center, const float radius) :center(center), radius(radius), a3Shape()
{

}

float a3Sphere::intersect(const a3Ray& ray) const
{
	// direction为单位向量，长度为1
	//float A = ray.direction * ray.direction;
    float B = ((ray.origin - center) * 2).dot(ray.direction);
	float C = (ray.origin - center).dot((ray.origin - center)) - radius*radius;

	float discriminant = B * B - 4.0f * C;

	//--!有局限 若测试线是人为指定 那么就可能出现origin在面上的情况 此时判别式不为0 但返回值为0
	if(discriminant < 0)
		return 0;
	else
		discriminant = sqrt(discriminant);

	float solution1 = -B + discriminant;
	float solution2 = -B - discriminant;

	// 舍弃与方向向量相反的解
	return (solution2 > A3_TOLERANCE_FLOAT) ? solution2 / 2 : ((solution1 > A3_TOLERANCE_FLOAT) ? solution1 / 2 : 0.0f);
}

t3Vector3f a3Sphere::getNormal(const t3Vector3f& hitPoint) const
{
    return (hitPoint - center).normalize();
}

void a3Sphere::print() const
{
#ifdef _DEBUG
    center.print("a3Sphere:center");

    std::cout << "a3Sphere:radius:" << radius << std::endl;

    a3Shape::print();
#endif
}

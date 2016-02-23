#ifndef T3_VECTOR4_H
#define T3_VECTOR4_H

template <class Type>
class t3Vector4
{
public:
	Type x, y, z, w;

	t3Vector4() :x(0), y(0), z(0), w(0)
	{
	}
	t3Vector4(const t3Vector4& v) :
		x(v.x), y(v.y), z(v.z), w(v.w)
	{
	}

	void set(const Type x, const Type y, const Type z, const Type w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

// 特化
typedef t3Vector4<float> t3Vector4f;
typedef t3Vector4<int> t3Vector4i;

// t2Point
typedef t3Vector4f t2Point4f;
typedef t3Vector4i t2Point4i;

// 零向量
static t3Vector4f t3Vector4fZero;
static t3Vector4i t3Vector4iZero;

#endif
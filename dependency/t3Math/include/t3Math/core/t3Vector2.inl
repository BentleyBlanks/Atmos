#ifndef T3_VECTOR2_INL
#define T3_VECTOR2_INL

#include <t3Math/core/t3Vector2.h>
#include <string>

template <class Type>
inline t3Vector2<Type>::t3Vector2() :x(0), y(0) {}

template <class Type>
inline t3Vector2<Type>::t3Vector2(Type x, Type y) : x(x), y(y) {}

// �����Ϳ�������
template <class Type>
inline t3Vector2<Type>::t3Vector2(const t3Vector2<Type>& vector) : x(vector.x), y(vector.y) {}

// t3Vector2<Type>->t3Vector2<Type2> explict��ֹ��ʽת��
template<typename Type>
template <typename Type2>
inline t3Vector2<Type>::t3Vector2(const t3Vector2<Type2> &vector) : x(vector.x), y(vector.y) {}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::clone(const t3Vector2<Type>& vector)
{
    x = vector.x; y = vector.y;
}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::set(t3Vector2& vector)
{
    set(vector.x, vector.y);
}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::set(const Type x, const Type y)
{
    this->x = x; this->y = y;
}

// ��Ϊ������
template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::zero()
{
    // --!֮���Դ˴���(Type)ǿ������ת�� ����Ϊ���岻�� ����ȴ���ıȽϴ���ٶ�
    x = 0;
    y = 0;
}

// ����2D������׼��
template <class Type>
T3_FORCE_INLINE bool t3Vector2<Type>::normalize()
{
    Type length = getLength();
    if(length == 0)
        return false;

    Type inverseLength = 1.0f / length;
    x *= inverseLength;
    y *= inverseLength;

    return true;
}

// ��ģ��
template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getLength() const
{
    return sqrtf(x*x + y*y);
}

template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getLengthSquare() const
{
    return (x*x + y*y);
}

// ���
template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::cross(const t3Vector2<Type>& vector) const
{
    // |i j k|
    // |a b c|
    // |x y z|
    // |axb|�ĳ��ȴ�������a��bΪ�ڱߵ�ƽ���ı��ε����
    return x*vector.y - y*vector.x;
}

// ��һ��������Ľ��Ϊ ����õ�ǰ�����Ĵ�ֱ����(-y, x)������scalar��
// ���
template <class Type>
T3_FORCE_INLINE t3Vector2<Type> t3Vector2<Type>::cross(const float& scalar) const
{
    return t3Vector2<Type>(-this->y*scalar, scalar*this->x);
}

template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::dot(const t3Vector2<Type>& vector) const
{
    return this->x*vector.x + this->y*vector.y;
}

// ��������
template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getDistance(const t3Vector2<Type>& end) const
{
    return sqrt((end.x - x)*(end.x - x) + (end.y - y)*(end.y - y));
}

template <class Type>
T3_FORCE_INLINE Type t3Vector2<Type>::getDistanceSquare(const t3Vector2<Type>& end) const
{
    return (end.x - x)*(end.x - x) + (end.y - y)*(end.y - y);
}

// ����ʹ�õ�Console Log���
template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::print(std::string name)
{
#ifdef _DEBUG
    // Ϊ��
    if(name.empty())
        name = "";
    std::cout << "t3Vector2 ";
    std::cout << name << "(x: " << x << ", y: " << y << ")" << std::endl;
#endif
}

template <class Type>
T3_FORCE_INLINE void t3Vector2<Type>::print()
{
#ifdef _DEBUG
    std::cout << "t3Vector2 ";
    std::cout << "(x: " << x << ", y: " << y << ")" << std::endl;
#endif
}

// ��Ա����
// ���� =
template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator=(const t3Vector2<Type>& vector)
{
    this->x = vector.x;
    this->y = vector.y;

    return *this;
}

// ���� == !=
template <class Type>
inline bool t3Vector2<Type>::operator==(const t3Vector2<Type>& vector) const
{
    return equals(this->x, vector.x) && equals(this->y, vector.y);
}

template <class Type>
inline bool t3Vector2<Type>::operator!=(const t3Vector2<Type>& vector) const
{
    return !(equals(this->x, vector.x) && equals(this->y, vector.y));
}

// ����һԪ - +
template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator-()
{
    t3Vector2<Type> v;
    v.set(-x, -y);
    return v;
}

template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator+()
{
    return *this;
}

// ���� ��Ԫ + -
template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator+(const t3Vector2<Type>& vector) const
{
    //--!�Ӽ��ź����ҳ�˳����ͬ
    return t3Vector2<Type>(x + vector.x, y + vector.y);
}

template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator-(const t3Vector2<Type>& vector) const
{
    return t3Vector2<Type>(x - vector.x, y - vector.y);
}

// ���� �˳��� * / scale
template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator*(const Type scale) const
{
    return t3Vector2<Type>(x * scale, y * scale);
}

template <class Type>
inline t3Vector2<Type> t3Vector2<Type>::operator/(const Type scale) const
{
    if(scale == 0)
    {
#ifdef _DEBUG
        // --!δ���滻Ϊlog
        std::cout << "��������Ϊ��!" << std::endl;
#endif
    }
    return t3Vector2<Type>(x / scale, y / scale);
}

// ���� += -= *= /=
template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator+=(const t3Vector2<Type>& vector)
{
    //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
    x += vector.x;
    y += vector.y;
    return *this;
}

template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator-=(const t3Vector2<Type>& vector)
{
    //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
    x -= vector.x;
    y -= vector.y;
    return *this;
}

template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator*=(const Type scale)
{
    //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
    x *= scale;
    y *= scale;
    return *this;
}

template <class Type>
inline t3Vector2<Type>& t3Vector2<Type>::operator/=(const Type scale)
{
    //--!֮���Բ�ʹ�����غ��+-*/��ϣ���ٵ���һ�κ��� ���ٿ���
    x /= scale;
    y /= scale;
    return *this;
}

// ���� * vector(����T3_FORCE_INLINEһ���ҳ� ��Ϊ������ͬ���Ҳ���)
template <class Type>
inline Type t3Vector2<Type>::operator*(const t3Vector2<Type>& vector) const
{
    return (x*vector.x + y*vector.y);
}


// �ǳ�Ա����
// ��˱���
// --!float���͵�Scale��Ϊ�˱���Scaleģ��������Martix���ͳ�ͻ���±������޷��ҵ���������
// template<class Scale, class Type>
template<class Type>
inline t3Vector2<Type> operator*(const float scale, const t3Vector2<Type>& vector)
{
    return t3Vector2<Type>(scale * vector.x, scale * vector.y);
}


// vector����һ����
// ʵ��Ϊ��vector����90�ȵõ�tempV ֮��������scalar��
template<class Type>
T3_FORCE_INLINE t3Vector2<Type> t2Cross(const Type scalar, const t3Vector2<Type>& vector)
{
    return t3Vector2<Type>(-scalar*vector.y, scalar*vector.x);
}

#endif
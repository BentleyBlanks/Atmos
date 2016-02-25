#include <t3Math/core/t3Matri4x4.h>
#include <memory.h>

// 特意不加分号让IDE能自动对齐……没什么区别
#define SET_ROW(row, v1, v2, v3, v4 )    \
	m[(row)][0] = (v1); \
	m[(row)][1] = (v2); \
	m[(row)][2] = (v3); \
	m[(row)][3] = (v4)

t3Matri4x4::t3Matri4x4()
{
	makeIdentity();
}

t3Matri4x4::t3Matri4x4(float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33)
{
	SET_ROW(0, a00, a01, a02, a03);
	SET_ROW(1, a10, a11, a12, a13);
	SET_ROW(2, a20, a21, a22, a23);
	SET_ROW(3, a30, a31, a32, a33);
}

t3Matri4x4::t3Matri4x4(float matrix[4][4])
{
    memcpy(m, matrix, 16 * sizeof(float));
}

t3Matri4x4::~t3Matri4x4()
{

}

void t3Matri4x4::set(float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33)
{
	SET_ROW(0, a00, a01, a02, a03);
	SET_ROW(1, a10, a11, a12, a13);
	SET_ROW(2, a20, a21, a22, a23);
	SET_ROW(3, a30, a31, a32, a33);
}

bool t3Matri4x4::isIdentity() const
{
	return m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f &&  m[0][3] == 0.0f &&
		m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f &&  m[1][3] == 0.0f &&
		m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f &&  m[2][3] == 0.0f &&
		m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f &&  m[3][3] == 1.0f;
}

void t3Matri4x4::makeIdentity()
{
	SET_ROW(0, 1, 0, 0, 0);
	SET_ROW(1, 0, 1, 0, 0);
	SET_ROW(2, 0, 0, 1, 0);
	SET_ROW(3, 0, 0, 0, 1);
}

void t3Matri4x4::makeOrtho2DMatrix(double left, double right, double bottom, double top)
{
	double zNear = -1.0, zFar = 1.0;

	double tx = -(right + left) / (right - left);
	double ty = -(top + bottom) / (top - bottom);
	double tz = -(zFar + zNear) / (zFar - zNear);
	SET_ROW(0, 2.0 / (right - left), 0.0, 0.0, 0.0);
	SET_ROW(1, 0.0, 2.0 / (top - bottom), 0.0, 0.0);
	SET_ROW(2, 0.0, 0.0, -2.0 / (zFar - zNear), 0.0);
	SET_ROW(3, tx, ty, tz, 1.0);
}


#ifndef T3_MATRIX4_H
#define T3_MATRIX4_H

class t3Matri4x4
{
public:
	t3Matri4x4();

    t3Matri4x4(float matrix[4][4]);

	t3Matri4x4(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33);

    ~t3Matri4x4();

	void set(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33);

	bool isIdentity() const;

	void makeIdentity();

	void makeOrtho2DMatrix(double left, double right, double bottom, double top);

	float m[4][4];
};

#endif
#ifndef T3_MATRIX4X4_H
#define T3_MATRIX4X4_H

#include <t3Math/core/t3Vector3.h>
#include <t3Math/core/t3Vector4.h>
#include <t3Math/core/t3Quaternion.h>
#include <cmath>

class t3Matrix4x4
{
public:
    //	float _mat[4][4];

    /// \brief The values of the matrix, stored in row-major order.
    t3Vector4f _mat[4];
    /// \cond INTERNAL
    // Should this be moved to private?
    /// \endcond

    /// \name Constructors
    /// \{

    /// \brief The default constructor provides an identity matrix.
    t3Matrix4x4()
    {
        makeIdentityMatrix();
    }

    /// \brief You can pass another t3Matrix4x4 to create a copy.
    t3Matrix4x4(const t3Matrix4x4& mat)
    {
        set(mat.getPtr());
    }

    /// \brief Construct with a pointer.
    /// 
    /// You can pass a pointer to floats, and the first 16 contents will be 
    /// extracted into this matrix.
    /// 
    /// \warning the validity of these values is not checked!
    t3Matrix4x4(float const * const ptr)
    {
        set(ptr);
    }

    /// \brief Rotation matrices can be constructed from a quaternion.
    t3Matrix4x4(const t3Quaternion& quat)
    {
        makeRotationMatrix(quat);
    }

    /// \brief Positional style.
    /// 
    /// All 16 values of the matrix as positional arguments in row-major order.
    t3Matrix4x4(float a00, float a01, float a02, float a03,
                float a10, float a11, float a12, float a13,
                float a20, float a21, float a22, float a23,
                float a30, float a31, float a32, float a33);

    /// \} end Constructor group

    /// \brief destructor.
    ~t3Matrix4x4() {}

    /// \name make* Methods
    /// \{
    /// These methods perform a total matrix makeover. All make* methods delete
    /// the current data and the matrix becomes a new type of transform.

    /// \brief Matrix becomes the identity matrix.
    void makeIdentityMatrix();

    /// \name Scale
    /// \{
    /// Matrix becomes a scale transform.
    /// 
    /// Accepts x, y, z scale values as a vector or separately.
    void makeScaleMatrix(const t3Vector3f&);
    void makeScaleMatrix(float, float, float);
    /// \}

    /// \name Translation
    /// \{
    /// Matrix becomes a translation transform.
    /// 
    /// Accepts x, y, z translation values as a vector or separately.
    void makeTranslationMatrix(const t3Vector3f&);
    void makeTranslationMatrix(float, float, float);
    /// \}

    /// \name Rotation
    /// \{
    /// Matrix becomes a rotation transform.
    /// 
    /// \param from Matrix becomes a rotation from this vector direction.
    /// \param to Matrix becomes a rotation to this vector direction.
    void makeRotationMatrix(const t3Vector3f& from, const t3Vector3f& to);
    /// \param angle Matrix becomes a rotation by angle (degrees).
    /// \param axis Rotation is performed around this vector.
    void makeRotationMatrix(float angle, const t3Vector3f& axis);
    /// \param angle Matrix becomes a rotation by angle (degrees).
    /// \param x X-value of the rotation axis.
    /// \param y Y-value of the rotation axis.
    /// \param z Z-value of the rotation axis.
    void makeRotationMatrix(float angle, float x, float y, float z);
    /// \param quaternion Matrix becomes a rotation that produces the quaternion's orientation.
    void makeRotationMatrix(const t3Quaternion& quaternion);
    /// \brief Matrix becomes a rotation around multiple axes.
    /// 
    /// The final rotation is the result of rotating around each of the three
    /// axes, in order. Angles are given in degrees, and axes can be arbitrary
    /// vectors.
    void makeRotationMatrix(float angle1, const t3Vector3f& axis1,
                            float angle2, const t3Vector3f& axis2,
                            float angle3, const t3Vector3f& axis3);
    /// \}


    /// \name Make* methods related to another matrix
    /// \{

    /// \brief Matrix becomes the inverse of the provided matrix.
    bool makeInvertOf(const t3Matrix4x4& rhs);

    /// \brief Matrix becomes an orthonormalized version of the provided matrix.
    /// 
    /// The basis vectors (the 3x3 chunk embedded in the upper left of the matrix)
    /// are normalized. This means the resulting matrix has had scaling effects
    /// removed. The fourth column and the fourth row are transferred over
    /// untouched, so translation will be included as well.
    void makeOrthoNormalOf(const t3Matrix4x4& rhs);

    /// \brief Matrix becomes the result of the multiplication of two other matrices.
    void makeFromMultiplicationOf(const t3Matrix4x4&, const t3Matrix4x4&);

    /// \}

    /// \name View matrix make*
    /// \{
    /// These functions are based on some OpenGL matrix functions used for
    /// perspective settings. See the OpenGL docs of the related function
    /// for further details.

    /// \brief Matrix becomes an orthographic projection matrix.
    /// 
    /// Related to: glOrtho. The orthographic projection has a box-shaped
    /// viewing volume described by the six parameters. Left, right, bottom,
    /// and top specify coordinates in the zNear clipping plane where the
    /// corresponding box sides intersect it.
    void makeOrthoMatrix(double left, double right,
                         double bottom, double top,
                         double zNear, double zFar);

    /// \brief Matrix becomes a 2D orthographic projection matrix.
    /// 
    /// Related to: glOrtho2D. The box-shaped viewing volume is 
    /// described by the four parameters and, implicitly, a zNear of -1 
    /// and a zFar of 1.
    void makeOrtho2DMatrix(double left, double right,
                           double bottom, double top);

    /// \brief Matrix becomes a perspective projection matrix.
    /// 
    /// Related to: glFrustum. The viewing volume is frustum-shaped and
    /// defined by the six parameters. Left, right, top, and bottom specify 
    /// coordinates in the zNear clipping plane where the frustum edges intersect
    /// it, and the zNear and zFar parameters define the forward distances of 
    /// the view volume. The resulting volume can be vertically and 
    /// horizontally asymmetrical around the center of the near plane.
    void makeFrustumMatrix(double left, double right,
                           double bottom, double top,
                           double zNear, double zFar);

    /// \brief Matrix becomes a perspective projection matrix.
    /// 
    /// Related to: gluPerspective. The viewing volume is frustum-shaped amd
    /// defined by the four parameters. The fovy and aspect ratio
    /// are used to compute the positions of the left, right, top, and bottom sides
    /// of the viewing volume in the zNear plane. The fovy is the y field-of-view,
    /// the angle made by the top and bottom sides of frustum if they were to
    /// intersect. The aspect ratio is the width of the frustum divided by its
    /// height. Note that the resulting volume is both vertically and 
    /// horizontally symmetrical around the center of the near plane.
    void makePerspectiveMatrix(double fovy, double aspectRatio,
                               double zNear, double zFar);

    /// \brief Matrix becomes a combination of translation and rotation.
    /// 
    /// Matrix becomes a combination of a translation to the position of 'eye'
    /// and a rotation matrix which orients an object to point towards 'center'
    /// along its z-axis. Use this function if you want an object to look at a 
    /// point from another point in space.
    /// 
    /// \param eye The position of the object.
    /// \param center The point which the object is "looking" at.
    /// \param up The direction which the object considers to be "up".
    void makeLookAtMatrix(const t3Vector3f& eye, const t3Vector3f& center, const t3Vector3f& up);

    /// \brief Matrix becomes a combination of an inverse translation and rotation.
    /// 
    /// Related to: gluLookAt. This creates the inverse of makeLookAtMatrix.
    /// The matrix will be an opposite translation from the 'eye' position,
    /// and it will rotate things in the opposite direction of the eye-to-center
    /// orientation. This is definitely confusing, but the main reason to use
    /// this transform is to set up a view matrix for a camera that's looking 
    /// at a certain point. To achieve the effect of moving the camera somewhere
    /// and rotating it so that it points at something, the rest of the world
    /// is moved in the *opposite* direction and rotated in the *opposite* way 
    /// around the camera. This way, you get the same effect as moving the actual
    /// camera, but all the projection math can still be done with the camera
    /// positioned at the origin (which makes it way simpler).
    void makeLookAtViewMatrix(const t3Vector3f& eye, const t3Vector3f& center, const t3Vector3f& up);

    /// \} end view-related group

    /// \} end make* group

    /// \name Static new* matrix functions
    /// \{
    /// 
    /// These are static utility functions to create new matrices. These
    /// functions generally return the equivalent of declaring a matrix and
    /// calling the corresponding "make..." function on it.

    /// \sa makeIdentityMatrix
    inline static t3Matrix4x4 newIdentityMatrix(void);

    /// \sa makeScaleMatrix
    inline static t3Matrix4x4 newScaleMatrix(const t3Vector3f& sv);
    inline static t3Matrix4x4 newScaleMatrix(float sx, float sy, float sz);

    /// \sa makeTranslationMatrix
    inline static t3Matrix4x4 newTranslationMatrix(const t3Vector3f& dv);
    inline static t3Matrix4x4 newTranslationMatrix(float x, float y, float z);

    /// \sa makeRotationMatrix
    inline static t3Matrix4x4 newRotationMatrix(const t3Vector3f& from, const t3Vector3f& to);
    inline static t3Matrix4x4 newRotationMatrix(float angle, float x, float y, float z);
    inline static t3Matrix4x4 newRotationMatrix(float angle, const t3Vector3f& axis);
    inline static t3Matrix4x4 newRotationMatrix(float angle1, const t3Vector3f& axis1,
                                                float angle2, const t3Vector3f& axis2,
                                                float angle3, const t3Vector3f& axis3);
    inline static t3Matrix4x4 newRotationMatrix(const t3Quaternion& quat);

    /// These functions create new matrices related to glFunctions. See 
    /// the description of the corresponding make* methods for more info.

    /// \sa makeOrthoMatrix
    inline static t3Matrix4x4 newOrthoMatrix(double left, double right,
                                             double bottom, double top,
                                             double zNear, double zFar);

    /// \sa makeOrtho2DMatrix
    inline static t3Matrix4x4 newOrtho2DMatrix(double left, double right,
                                               double bottom, double top);

    /// \sa makeFrustumMatrix
    inline static t3Matrix4x4 newFrustumMatrix(double left, double right,
                                               double bottom, double top,
                                               double zNear, double zFar);

    /// \sa makePerspectiveMatrix
    inline static t3Matrix4x4 newPerspectiveMatrix(double fovy, double aspectRatio,
                                                   double zNear, double zFar);

    /// \sa makeLookAtMatrix
    inline static t3Matrix4x4 newLookAtMatrix(const t3Vector3f& eye,
                                              const t3Vector3f& center,
                                              const t3Vector3f& up);

    /// \} end Static new* matrix group

    //---------------------
    /// \name Accessors
    /// \{

    /// \brief Write data with `matrix(row,col)=number`
    float& operator()(std::size_t row, std::size_t col)
    {
        return _mat[row][col];
    }

    /// \brief Read data with `matrix(row, col)`
    float operator()(std::size_t row, std::size_t col) const
    {
        return _mat[row][col];
    }

    /// \brief returns a copy of row i
    t3Vector3f getRowAsVec3f(std::size_t i) const
    {
        return t3Vector3f(_mat[i][0], _mat[i][1], _mat[i][2]);
    }

    /// \brief returns a copy of row i
    t3Vector4f getRowAsVec4f(std::size_t i) const
    {
        return _mat[i];
    }

    /// \cond INTERNAL
    friend std::ostream& operator<<(std::ostream& os, const t3Matrix4x4& M);
    friend std::istream& operator>>(std::istream& is, t3Matrix4x4& M);
    /// \endcond

    /// \brief Access the internal data in `float*` format
    /// useful for opengl matrix transformations
    float * getPtr()
    {
        return (float*) _mat;
    }
    const float * getPtr() const
    {
        return (const float *) _mat;
    }

    /// \} end accessors group

    //---------------------
    /// \name Checking
    /// \{

    /// \brief Checks if the matrix is valid by ensuring its items are numbers.
    bool isValid() const
    {
        return !isNaN();
    }

    /// \brief Checks if the matrix contains items that are not numbers.
    bool isNaN() const;

    /// \brief Checks if the matrix is the identity matrix.
    bool isIdentity() const;

    /// \}

    //---------------------
    /// \name Setters
    /// \{

    /// \brief Copy a matrix using `=` operator.
    t3Matrix4x4& operator = (const t3Matrix4x4& rhs);


    /// \brief Set the data of the matrix.
    /// 
    /// These functions are analogous to the corresponding constructors.
    void set(const t3Matrix4x4& rhs);
    void set(float const * const ptr);
    void set(double const * const ptr);
    void set(float a00, float a01, float a02, float a03,
             float a10, float a11, float a12, float a13,
             float a20, float a21, float a22, float a23,
             float a30, float a31, float a32, float a33);

    /// \}

    //---------------------
    /// \name Getters
    /// \{

    /// \brief Gets the inverse matrix.
    t3Matrix4x4 getInverse() const;

    /// \brief Get the perspective components from a matrix.
    /// 
    /// This only works with pure perspective projection matrices.
    bool getOrtho(double& left, double& right,
                  double& bottom, double& top,
                  double& zNear, double& zFar) const;

    /// \brief Gets the perspective components for a frustum projection matrix.
    bool getFrustum(double& left, double& right,
                    double& bottom, double& top,
                    double& zNear, double& zFar) const;

    /// \brief Get the frustum settings of a symmetric perspective projection
    /// matrix.
    /// 
    /// Note, if matrix is not a symmetric perspective matrix then the
    /// shear will be lost.
    /// Asymmetric matrices occur when stereo, power walls, caves and
    /// reality center display are used.
    /// In these configuration one should use the getFrustum method instead.
    ///
    /// \returns false if matrix is not a perspective matrix,
    /// where parameter values are undefined.
    bool getPerspective(double& fovy, double& aspectRatio,
                        double& zNear, double& zFar) const;

    /// \brief Gets the lookAt determiners of the matrix.
    /// 
    /// This function will only work for modelview matrices.
    void getLookAt(t3Vector3f& eye, t3Vector3f& center, t3Vector3f& up,
                   float lookDistance = 1.0f) const;

    /// \brief Decompose the matrix into translation, rotation,
    /// scale and scale orientation.
    void decompose(t3Vector3f& translation,
                   t3Quaternion& rotation,
                   t3Vector3f& scale,
                   t3Quaternion& so) const;

    /// Create new matrices as transformation of another.

    /// \brief Makes a new matrix which is the inverse of the given matrix.
    inline static t3Matrix4x4 getInverseOf(const t3Matrix4x4& matrix);
    /// \brief Makes a new matrix which is the transpose of the given matrix.
    inline static t3Matrix4x4 getTransposedOf(const t3Matrix4x4& matrix);
    /// \brief Makes a new matrix which is the given matrix, normalized.
    inline static t3Matrix4x4 getOrthoNormalOf(const t3Matrix4x4& matrix);


    /// \}

    //---------------------
    /// \name Matrix multiplication
    /// \{

    /// Although OpenGL uses post-multiplication (vector-on-the-right) with
    /// column-major matrix memory layout, oF uses pre-multiplication
    /// (vector-on-the-left) with row-major matrix memory layout by default.
    /// 
    /// openGL:
    ///
    /// |   |   |   |   |
    /// |:-:|:-:|:-:|:-:|
    /// | 0 | 4 | 8 | 12|
    /// | 1 | 5 | 9 | 13|
    /// | 2 | 6 | 10| 14|
    /// | 3 | 7 | 11| 15|
    /// 
    /// t3Matrix4x4:
    /// 
    /// |   |   |   |   |
    /// |:-:|:-:|:-:|:-:|
    /// | 0 | 1 | 2 | 3 |
    /// | 4 | 5 | 6 | 7 |
    /// | 8 | 9 | 10| 11|
    /// | 12| 13| 14| 15|
    ///
    /// However, the two memory layouts are compatible because of a funny trick.
    /// 
    /// When the t3Matrix4x4 is uploaded into OpenGL's memory, OpenGL treats it
    /// like a column-major matrix. The rows of the t3Matrix4x4 are loaded as
    /// columns for the GLSL mat4. The result is that the matrix is transposed.
    /// This seems like a bug, but it's in fact exactly what we want, because to
    /// do the transition from pre-multiplication to post-multiplication style,
    /// we need to perform the very same transpose.
    /// 
    /// By using pre-multiplication, oF treats vectors as 1x4 matrices, since 
    /// that provides a valid 1x4 * 4x4 operation. When moving to
    /// post-multiplication, OpenGL is treating vectors like columns, 
    /// providing a similarly valid 4x4 * 4x1 operation. This means that the
    ///  resulting vector in OGL-land is the transpose of the result when
    /// done in oF-land.
    /// 
    /// Recall that in matrix multiplication,
    /// 
    /// (V * M * S)^T = (S^T) * (M^T) * (V^T)
    /// 
    /// What this means is that to convert from pre-multiplication to 
    /// post-multiplication, we need to transpose our matrices (and vectors) 
    /// and reverse the order of multiplication. You're already reversing
    /// the order of multiplication by writing your shaders with the vector
    /// on the right, and the implicit transpose that happens when your matrix
    /// is uploaded to GL memory accomplishes the transposition for free!
    /// 
    /// For more information on this subject, check out
    /// [this post](http://seanmiddleditch.com/matrices-handedness-pre-and-post-multiplication-row-vs-column-major-and-notations/).

    /// \brief Matrix * vector multiplication.
    /// 
    /// This operation implicitly treat vectors as column-matrices.
    inline t3Vector3f postMult(const t3Vector3f& v) const;
    inline t3Vector4f postMult(const t3Vector4f& v) const;

    /// \brief Post-multiply by another matrix.
    /// 
    /// This matrix becomes `this * other`.
    void postMult(const t3Matrix4x4&);

    /// \brief Vector * matrix multiplication.
    /// 
    /// This operation implicitly treats vectors as row-matrices.
    inline t3Vector3f preMult(const t3Vector3f& v) const;
    inline t3Vector4f preMult(const t3Vector4f& v) const;

    /// \brief Pre-multiply by another matrix.
    /// 
    /// This matrix becomes `other * this`.
    void preMult(const t3Matrix4x4&);

    /// \brief The *= operation for matrices.
    /// 
    /// This is equivalent to calling postMult(other), but it allows you to do
    /// someMatrix *= someMatrix without breaking const-correctness. Calling
    /// someMatrix.postMult(someMatrix) won't work.
    inline void operator *= (const t3Matrix4x4& other)
    {
        if(this == &other)
        {
            t3Matrix4x4 temp(other);
            postMult(temp);
        }
        else postMult(other);
    }

    /// \brief creates a new matrix from the product of two matrices.
    inline t3Matrix4x4 operator * (const t3Matrix4x4 &m) const
    {
        t3Matrix4x4 r;
        r.makeFromMultiplicationOf(*this, m);
        return  r;
    }

    /// \brief Matrix * Vector operator.
    /// 
    /// Calls postMult() internally.
    inline t3Vector3f operator* (const t3Vector3f& v) const
    {
        return postMult(v);
    }

    inline t3Vector4f operator* (const t3Vector4f& v) const
    {
        return postMult(v);
    }

    /// These are specialized postMult methods, usually you want to use these
    /// for transforming with ofVec.

    /// \brief Equivalent to postMult(newTranslationMatrix(v)).
    inline void postMultTranslate(const t3Vector3f& v);
    /// \brief Equivalent to postMult(scale(v)).
    inline void postMultScale(const t3Vector3f& v);
    /// \brief Equivalent to postMult(newRotationMatrix(q)).
    inline void postMultRotate(const t3Quaternion& q);

    inline void postMultTranslate(float x, float y, float z);
    inline void postMultRotate(float angle, float x, float y, float z);
    inline void postMultScale(float x, float y, float z);

    /// \brief Equivalent to preMult(newScaleMatrix(v)).
    inline void preMultScale(const t3Vector3f& v);
    /// \brief Equivalent to preMult(newTranslationMatrix(v)).
    inline void preMultTranslate(const t3Vector3f& v);
    /// \brief Equivalent to preMult(newRotationMatrix(q)).
    inline void preMultRotate(const t3Quaternion& q);

    /// \}

    //---------------------
    /// \name Matrix transformation
    /// \{

    /// \name Set methods
    /// \{
    /// 
    /// All of these methods alter the components,
    /// deleting the previous data only in that component.
    void setRotate(const t3Quaternion& q);
    void setTranslation(float tx, float ty, float tz);
    void setTranslation(const t3Vector3f& v);
    /// \}

    /// \name PostMult Transformations
    /// \{
    /// 
    /// All of these methods apply the transformations over the current one,
    /// calling postMult under the hood. These work as the opposite of the 
    /// equivalent OpenGL functions.
    /// 
    /// For example, glTranslate + glRotate == rotate + translate.

    /// \brief Rotates by angle (degrees) around the given x, y, z axis.
    void rotate(float angle, float x, float y, float z);
    /// \brief Rotates by angle (radians) around the given x, y, z axis.
    void rotateRad(float angle, float x, float y, float z);
    /// \brief Rotates based on the quarternion.
    void rotate(const t3Quaternion& q);
    /// \brief Translates by tx, ty, tz.
    void translate(float tx, float ty, float tz);
    /// \brief Translates along the vector.
    void translate(const t3Vector3f& v);
    /// \brief Scales each axis by the corresponding x, y, z.
    void scale(float x, float y, float z);
    /// \brief Scales each axis by the corresponding x, y, z of the vector.
    void scale(const t3Vector3f& v);
    /// \}

    /// \name PreMult Transformations
    /// \{
    /// 
    /// All of these methods apply the transformations over the current one,
    /// calling preMult under the hood. These work the the same the equivalent 
    /// OpenGL functions.

    /// \sa rotate
    void glRotate(float angle, float x, float y, float z);
    /// \sa rotate
    void glRotateRad(float angle, float x, float y, float z);
    /// \sa rotate
    void glRotate(const t3Quaternion& q);
    /// \sa translate
    void glTranslate(float tx, float ty, float tz);
    /// \sa translate
    void glTranslate(const t3Vector3f& v);
    /// \sa scale
    void glScale(float x, float y, float z);
    /// \sa scale
    void glScale(const t3Vector3f& v);
    /// \}

    /// \name Get Methods
    /// \{
    /// 
    /// These return matrix components. getRotate and getScale can only be 
    /// used if the matrix only has rotation or only has scale, since these
    /// transform values are stored in the same area of the matrix.
    /// For matrices with both use decompose instead.
    t3Quaternion getRotate() const;
    t3Vector3f getTranslation() const;
    t3Vector3f getScale() const;
    /// \}

    /// \brief Apply a 3x3 transform (no translation) of v * M.
    inline static t3Vector3f transform3x3(const t3Vector3f& v, const t3Matrix4x4& m);

    /// \brief Apply a 3x3 transform (no translation) of M * v.
    inline static t3Vector3f transform3x3(const t3Matrix4x4& m, const t3Vector3f& v);

    /// \}

};

#include <t3Math/core/t3Matrix4x4.inl>

#endif
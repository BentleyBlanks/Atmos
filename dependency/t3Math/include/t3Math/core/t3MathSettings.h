#define T3_VARARGS     __cdecl					/* Functions with variable arguments */
#define T3_CDECL	    __cdecl					/* Standard C function */
#define T3_STDCALL		__stdcall				/* Standard calling convention */
#define T3_FORCE_INLINE __forceinline			/* Force code to be inline */
#define T3_FORCE_NOINLINE __declspec(noinline)	/* Force code to NOT be inline */

#define T3MATH_DEFAULT_EPSILON 0.000001
#define T3MATH_LARGE_EPSILON 0.00001

#define T3MATH_E 2.71828182846
#define T3MATH_PI 3.14159265358979323

#define T3MATH_INV_PI     0.31830988618379067154f
#define T3MATH_INV_TWOPI  0.15915494309189533577f
#define T3MATH_INV_FOURPI 0.07957747154594766788f

#ifndef T3MATH_INFINITY
#define T3MATH_INFINITY FLT_MAX
#endif

#define T3MATH_RAD_TO_DEG (180.0 / T3MATH_PI)

#define T3MATH_DEG_TO_RAD (T3MATH_PI / 180.0)
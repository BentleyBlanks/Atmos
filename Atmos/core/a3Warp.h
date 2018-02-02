#pragma once

#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>
#include <core/a3Spectrum.h>

// Public Function for convenient computation
// ----------------------------------------------Sampling----------------------------------------------
enum a3UniformSampleDiskType
{
    // --!Peter Shirley's solution https://mediatech.aalto.fi/~jaakko/T111-5310/K2013/JGT-97.pdf
    A3UNIFORM_SAMPLE_DISK_CONCENTRIC = 0,
    // common solution to uniform sample a disk
    A3UNIFORM_SAMPLE_DISK_CARTESIAN
};

enum a3CosineSampleHemisphereType
{
    // uniform sample a disk then projectd to hemisphere to cosine-weighted sample a hemisphere
    A3UNIFORM_SAMPLE_HEMISPHERE_PROJECTED = 0,
    // common solution to cosine-weighted sample a hemisphere
    A3UNIFORM_SAMPLE_HEMISPHERE_CARTESIAN
};

// Uniformly sample a vector on a 2D disk
t3Vector2f a3UniformSampleDisk(float u1, float u2, a3UniformSampleDiskType type = A3UNIFORM_SAMPLE_DISK_CONCENTRIC);

// Uniformly sample a vector on the unit sphere with respect to solid angles
t3Vector3f a3UniformSampleSphere(float u1, float u2);

// Uniformly sample a vector on the unit hemisphere with respect to solid angles
t3Vector3f a3UniformSampleHemisphere(float u1, float u2);

// Sample a cosine-weighted vector on the unit hemisphere with respect to solid angles
t3Vector3f a3CosineSampleHemisphere(float u1, float u2, a3CosineSampleHemisphereType type = A3UNIFORM_SAMPLE_HEMISPHERE_PROJECTED);

// sampled cone uniformly with square sample
t3Vector3f a3UniformSampleCone(float u1, float u2, float cosThetaMax);

// Convert an uniformly distributed square sample into barycentric coordinates
t3Vector2f a3UniformSampleTriangle(float u1, float u2);

// uniform sample a normalized sphere
float a3UniformSpherePdf();

// uniform sample a normalized hemisphere
float a3UniformHemispherePdf();

// Density of a3CosineSampleHemisphere() with respect to solid angles
float a3CosineSampleHemispherePdf(const t3Vector3f& d);



// ----------------------------------------------Post Effect----------------------------------------------
// map one set of colors to another to approximate the appearance of high dynamic range image
void a3ToneMapping(t3Vector3f* colorList, int startX, int startY, int localWidth, int localHeight, int width, int height);

// map one set of colors to another to approximate the appearance of high dynamic range image
void a3ToneMapping(t3Vector3f* colorList, int width, int height);

// map one set of colors to another to approximate the appearance of high dynamic range image
void a3ToneMapping(float* buffer, int width, int height);

// the tonemapping used in Unchared
void a3UncharedTonemap(t3Vector3f& color, float eyeAdaption = 4);

// do gamma correction to convert linear space to srgb
void a3GammaCorrection(float& r, float&g, float& b);

// do gamma correction to convert linear space to srgb
void a3GammaCorrection(t3Vector3f& color);

// convert linear rgb component to srgb
float a3RGBToSRGBComponent(float value);



// ----------------------------------------------Math----------------------------------------------
// spherical coordinate's theta:[0, 2pi]
float a3SphericalTheta(const t3Vector3f &w);

// spherical coordinate's phi:[-pi, +pi]
float a3SphericalPhi(const t3Vector3f &w);

// spherical coordinate's cosphi
float a3SphericalCosPhi(const t3Vector3f &w);

// spherical coordinate's cosphi
float a3SphericalSinPhi(const t3Vector3f &w); 

// spherical coordinate's cos2phi
float a3SphericalCos2Phi(const t3Vector3f &w);

// spherical coordinate's sin2phi
float a3SphericalSin2Phi(const t3Vector3f &w);

// get a orthonomal system, the v2 v3 are perpendicular to v1
void a3OrthonomalSystem(const t3Vector3f& v1, t3Vector3f& v2, t3Vector3f& v3);

// solving quadratic(exist a solution if t0 <= t1)
bool a3SolveQuadratic(float A, float B, float C, float* t0, float* t1);

// solving quadratic(exist a solution if t0 <= t1)
bool a3SolveQuadraticDouble(double A, double B, double C, double* t0, double* t1);

// Multiple importance sampling's herostic function
float a3MiWeight(float fPdf, float gPdf);

// convert fov to apretureWidth/Height(Degree)
float a3FovToApretureSizeDeg(float fov);

// convert fov to apretureWidth/Height(Radians)
float a3FovToApretureSizeRad(float fov);

// conbert RGBSpectrum to Luminance
float a3RGBToLuminance(const a3Spectrum& rgb);

// Calculates the unpolarized Fresnel reflection coefficient 
// at a planar interface having a complex - valued relative index of refraction
a3Spectrum a3FresnelConductor(float cosThetai, const a3Spectrum& eta, const a3Spectrum& k);

// Calculates the unpolarized Fresnel reflection coefficient
// at a planar interface between two dielectrics
float a3FresnelDielectric(float cosThetaI, float& cosThetaT, float eta);

// get costheta in shading coordinate
float a3CosTheta(const t3Vector3f& w);

// get costheta * costheta in shading coordinate
float a3Cos2Theta(const t3Vector3f& w);

// get sintheta in shading coordinate
float a3SinTheta(const t3Vector3f& w);

// get sintheta * sintheta in shading coordinate
float a3Sin2Theta(const t3Vector3f& w);

// get tantheta in shading coordinate
float a3TanTheta(const t3Vector3f& w);

// get tantheta * tantheta in shading coordinate
float a3Tan2Theta(const t3Vector3f& w);



// ----------------------------------------------Geometry----------------------------------------------
// get reflect vector in shading coordinates
t3Vector3f a3GetReflect(const t3Vector3f& a);

// giving surface normal to get reflect vector 
t3Vector3f a3GetReflect(const t3Vector3f& a, const t3Vector3f& normal);

// get refract vector in shading coordinates
// eta = interiorEta / exteriorEta
t3Vector3f a3GetRefract(const t3Vector3f& wi, float cosThetaT, float eta);


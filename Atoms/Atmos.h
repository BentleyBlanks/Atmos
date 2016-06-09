// shape
#include <shapes/a3Sphere.h>
#include <shapes/a3Plane.h>
#include <shapes/a3Disk.h>
#include <shapes/a3Triangle.h>
#include <shapes/a3InfinitePlane.h>
#include <shapes/a3TriangleMesh.h>

#include <Common/t3Timer.h>

// light
#include <lights/a3InfiniteAreaLight.h>
#include <lights/a3PointLight.h>
#include <lights/a3SpotLight.h>
#include <lights/a3AreaLight.h>

// renderer
#include <renderers/a3SamplerRenderer.h>
#include <renderers/a3NormalMapRenderer.h>
#include <renderers/a3SingleRayRenderer.h>
#include <renderers/a3FresnelRenderer.h>
#include <renderers/a3GridRenderer.h>

// textures
#include <textures/a3ImageTexture.h>
#include <textures/a3ConstantTexture.h>
#include <textures/a3CheckerBoard.h>

#include <accelerators/a3BVH.h>
#include <accelerators/a3Exhaustive.h>
#include <samples/a3RandomSampler.h>
#include <sensors/a3PerspectiveSensor.h>

//integrator
#include <integrator/a3PathIntegrator.h>
#include <integrator/a3DirectLighting.h>

// bsdf
#include <bsdf/a3Diffuse.h>
#include <bsdf/a3Conductor.h>
#include <bsdf/a3Dielectric.h>

// core
#include <core/log/a3Log.h>
#include <core/a3ModelImporter.h>
#include <core/a3Common.h>
#include <core/image/a3NormalMap.h>
#include <core/image/a3Film.h>
#include <core/a3Scene.h>
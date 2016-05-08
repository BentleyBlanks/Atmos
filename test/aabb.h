#include <core/log/a3Log.h>
#include <core/a3AABB.h>
#include <core/a3Ray.h>

int main()
{
    a3AABB box0(t3Vector3f(0), t3Vector3f(10)),
           box1(t3Vector3f(11, 0, 0), t3Vector3f(20, 10, 10)),
           box2(t3Vector3f(-5, 0, 0), t3Vector3f(5, 10, 10));

    a3Ray ray(t3Vector3f(0), t3Vector3f(1));

    t3Vector3f p(-1);

    // overlaps
    if(box0.overlaps(box1))
        a3Log::print("box0 overlaps box1\n");
    else
        a3Log::print("box0 not overlaps box1\n");

    if(box0.overlaps(box2))
        a3Log::print("box0 overlaps box2\n");
    else
        a3Log::print("box0 not overlaps box2\n");

    // inside
    if(box0.inside(p))
        a3Log::print("p inside box0\n");
    else
        a3Log::print("p outside box0\n");

    // expand
    a3AABB aabb0(t3Vector3f(0), t3Vector3f(2)),
           aabb1(t3Vector3f(1, 2, 3), t3Vector3f(9, 8, 7));
    aabb0.print("aabb0");
    aabb0.expand(5);
    aabb0.print("aabb0");

    // surface area
    a3Log::print("area: %f\n", aabb0.surfaceArea());

    // volume
    a3Log::print("volume: %f\n", aabb0.volume());

    // maxExtent
    a3Log::print("max extent index: %d\n", aabb1.maxExtentIndex());

    // lerp
    box0.interpolate(0.5f, 0.5f, 0.5f).print("interpolate point");

    // sphere
    t3Vector3f center;
    float radius = 0;
    box0.boundingSphere(&center, &radius);
    center.print("center");
    a3Log::print("radius: %f\n", radius);

    // intersect
    float hitT0 = 0, hitT1 = 0;
    if(box0.intersect(ray, &hitT0, &hitT1))
        a3Log::print("ray intersect box0 t0: %f, t1: %f\n", hitT0, hitT1);
    else
        a3Log::print("ray not intersect box0\n");

    getchar();
    return 0;
}
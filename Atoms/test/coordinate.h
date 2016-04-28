#include <core/a3Settings.h>
#include <core/a3Warp.h>
#include <core/log/a3Log.h>

#include <t3Math/core/t3Matri4x4.h>
#include <t3Math/core/t3Vector3.h>
int main()
{
    t3Matrix4x4 lightToWorld, worldToLight;
    
    t3Vector3f pos(0, 0, 0), dir(1, 1, 1);

    // 光源局部坐标系下的点
    t3Vector3f p(1, 0, 0);

    t3Vector3f du, dv;

    a3OrthonomalSystem(dir, du, dv);

    lightToWorld.set(du.x,  du.y,  du.z,  0,
                     dv.x,  dv.y,  dv.z,  0,
                     dir.x, dir.y, dir.z, 0,
                     pos.x, pos.y, pos.z, 1);

    t3Vector3f pWorld = p * lightToWorld;

    a3Log::print("--------------------------------\n");

    du.print("x");
    dv.print("y");
    dir.print("z");

    a3Log::print("\n--------------------------------\n");

    p.print("Local Position");
    pWorld.print("World Position");

    getchar();
    return 0;
}
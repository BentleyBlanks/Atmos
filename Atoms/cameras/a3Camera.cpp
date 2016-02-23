#include <cameras/a3Camera.h>
#include <film/a3Image.h>


a3Camera::a3Camera(const t3Vector3f& origin, const t3Vector3f& direction, float fov, a3Image* image) 
: origin(origin), direction(direction), image(image)
{
    
}

#include <core/image/a3NormalMap.h>
#include <core/image/a3ImageEncoder.h>
#include <samples/a3CameraSample.h>

a3NormalMap::a3NormalMap(unsigned width, unsigned height, const std::string& fileName, a3ImageType type /*= A3_IMAGE_PNG*/) : a3Film(width, height, fileName, type)
{

}

a3NormalMap::a3NormalMap(const a3Film& film) : a3Film(film.width, film.height, film.fileName, film.picture->type)
{

}

a3NormalMap::~a3NormalMap()
{

}

void a3NormalMap::addSample(const a3CameraSample* sample, const t3Vector3f& normal)
{
    t3Vector3f normalTemp = normal.getNormalized();

    normalTemp = (normalTemp / 2.0f + t3Vector3f(0.5f, 0.5f, 0.5f)) * 255;

    picture->setColor(sample->imageX, sample->imageY, normalTemp);
}

#include <core/image/a3Film.h>
#include <samples/a3CameraSample.h>
#include <core/log/a3Log.h>

#include <core/image/a3ImageEncoder.h>

a3Film::a3Film(unsigned width, unsigned height, const std::string& fileName, a3ImageType type) : width(width), height(height), fileName(fileName)
{
    picture = new a3ImageEncoder(width, height, type);
}

a3Film::~a3Film()
{
    delete picture;
}

void a3Film::write()
{
    picture->write(fileName);
}

void a3Film::addSample(const a3CameraSample* sample, const t3Vector3f& L)
{
    int x = sample->imageX, y = sample->imageY;

    picture->setColor(x, y, L);
}

void a3Film::setFileName(const std::string& fileName)
{
    this->fileName = fileName;
}

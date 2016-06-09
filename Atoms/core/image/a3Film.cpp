#include <core/image/a3Film.h>
#include <samples/a3CameraSample.h>
#include <core/log/a3Log.h>

#include <core/image/a3ImageEncoder.h>

a3Film::a3Film(unsigned width, unsigned height, const std::string& fileName) : width(width), height(height), fileName(fileName)
{
    a3ImageType type;

    char extension[32];
    _splitpath(fileName.c_str(), NULL, NULL, NULL, extension);

    // 根据后缀解析文件格式
    if(!_stricmp(extension, ".png"))
        type = A3_IMAGE_PNG;
    else if(!_stricmp(extension, ".exr"))
        type = A3_IMAGE_EXR;
    else if(!_stricmp(extension, ".ppm"))
        type = A3_IMAGE_PPM;
    else
        type = A3_IMAGE_ERROR;

    picture = new a3ImageEncoder(width, height, type);
}

a3Film::~a3Film()
{
    A3_SAFE_DELETE(picture);
}

void a3Film::addColor(int x, int y, const t3Vector3f& color)
{
    picture->setColor(x, y, color);
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

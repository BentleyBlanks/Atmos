#include <core/image/a3ImageDecoder.h>
#include <core/log/a3Log.h>
#include <core/a3BilinearInterpolation.h>

#ifdef A3_IMAGE_LIB_PNGPP
#include <png.hpp>

#elif defined A3_IMAGE_LIB_LODEPNG
#include <lodepng.h>
#endif

class a3ImageDecoder::a3Decoder
{
public:
    bool load(const std::string& filePath)
    {
        unsigned error = lodepng::decode(image, width, height, filePath);

        if(error)
        {
            a3Log::error("a3Decoder error %d: %s\n", error, lodepng_error_text(error));
            return false;
        }
        else
        {
            a3Log::success("Image %s load succeed\n", filePath.c_str());
            a3Log::success("width:%d, height:%d\n", width, height);
            return true;
        }
    }

    t3Vector3f getColor(float x, float y) const
    {
        if(x < 0 || x > width || y < 0 || y > height)
        {
            a3Log::warning("a3Decoder error coordinate: x:%d, y:%d\n", x, y);
            return t3Vector3f::zero();
        }

        int floorX = t3Math::floor(x), floorY = t3Math::floor(y);

        // 取(x, y)所在[0, 1]区间 
        int ceilX = t3Math::ceil(x), ceilY = t3Math::ceil(y);
        if(ceilX == x)
            ceilX = x + 1;
        if(ceilY == y)
            ceilY = y + 1;

        static float f00, f10, f01, f11;
        static float color[3];

        for(int i = 0; i < 3; i++)
        {
            f00 = image[(floorX + floorY * width) * 4 + i];
            f10 = image[(ceilX + floorY * width) * 4 + i];
            f11 = image[(ceilX + ceilY * width) * 4 + i];
            f01 = image[(floorX + ceilY * width) * 4 + i];

            color[i] = a3BilinearInterpolation(x - floorX, y - floorY, f00, f01, f11, f10);
        }

        return t3Vector3f(color[0], color[1], color[2]);
        // 舍弃alpha值
        //return t3Vector3f(image[(x + y * width) * 4 + 0], image[(x + y * width) * 4 + 1], image[(x + y * width) * 4 + 2]);
    }

    unsigned int getWidth() const
    {
        return width;
    }

    unsigned int getHeight() const
    {
        return height;
    }

    void print()
    {
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                a3Log::print("[%d]", i * 10 + j);
                a3Log::print("r: %d, ", image[i * 10 + j + 0]);
                a3Log::print("g: %d, ", image[i * 10 + j + 1]);
                a3Log::print("b: %d, ", image[i * 10 + j + 2]);
                a3Log::print("a: %d\n", image[i * 10 + j + 3]);
            }
        }
    }

#ifdef A3_IMAGE_LIB_PNGPP
    png::image< png::rgb_pixel >* im;
#else
    std::vector<unsigned char> image;
#endif

    unsigned width, height;
};

a3ImageDecoder::a3ImageDecoder()
{
    decoder = new a3Decoder();
}

a3ImageDecoder::a3ImageDecoder(const std::string& filePath)
{
    decoder = new a3Decoder();
    load(filePath);
}

a3ImageDecoder::~a3ImageDecoder()
{
    delete decoder;
}

bool a3ImageDecoder::load(const std::string& filePath)
{
    bLoaded = decoder->load(filePath);
    return bLoaded;
}

t3Vector3f a3ImageDecoder::getColor(int x, int y) const
{
    return decoder->getColor(x, y);
}

t3Vector3f a3ImageDecoder::lookup(float u, float v) const
{
    return decoder->getColor(u * (decoder->width - 1), v * (decoder->height - 1));
}

void a3ImageDecoder::print()
{
    decoder->print();
}

bool a3ImageDecoder::isLoaded() const
{
    return bLoaded;
}

int a3ImageDecoder::getWidth() const
{
    return decoder->getWidth();
}

int a3ImageDecoder::getHeight() const
{
    return decoder->getHeight();
}

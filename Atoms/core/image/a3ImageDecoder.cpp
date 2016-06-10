#include <core/image/a3ImageDecoder.h>
#include <core/log/a3Log.h>
#include <core/a3BilinearInterpolation.h>
#include <core/a3Warp.h>
#include <string.h>

#ifdef A3_IMAGE_LIB_PNGPP
#include <png.hpp>

#elif defined A3_IMAGE_LIB_LODEPNG
#include <lodepng.h>
#endif

#ifdef A3_IMAGE_LIB_TINYEXR
#include <tinyexr.h>
#endif

class a3ImageDecoder::a3Decoder
{
public:
    ~a3Decoder()
    {
#ifdef A3_IMAGE_LIB_PNGPP
        A3_SAFE_DELETE(im);
#else
#endif
        image.clear();

        A3_SAFE_DELETE_1DARRAY(buffer);
    }

    bool load(const std::string& filePath)
    {
        if(type == A3_IMAGE_PNG)
        {
            unsigned error = lodepng::decode(image, width, height, filePath);

            if(error)
            {
                a3Log::error("a3Decoder::load() error %d: %s\n", error, lodepng_error_text(error));
                return false;
            }
            else
            {
                a3Log::success("Image %s load succeed\n", filePath.c_str());
                a3Log::success("width:%d, height:%d\n", width, height);
                return true;
            }
        }
        else if(type == A3_IMAGE_EXR)
        {
            const char* error;
            if(LoadEXR(&buffer, (int*)(&width), (int*)(&height), filePath.c_str(), &error) == -1)
            {
                a3Log::error("a3Decoder::load() error %s\n", error);
                buffer = NULL;
                return false;
            }
            else
            {
                // exr image need gamma correction
                for(int x = 0; x < width; x++)
                {
                    for(int y = 0; y < height; y++)
                    {
                        float &r = buffer[(x * height + y) * 4 + 0],
                              &g = buffer[(x * height + y) * 4 + 1],
                              &b = buffer[(x * height + y) * 4 + 2];

                        a3GammaCorrection(r, g, b);
                    }
                }

                a3Log::success("Image %s load succeed\n", filePath.c_str());
                a3Log::success("width:%d, height:%d\n", width, height);
                return true;
            }
        }
        else if(type == A3_IMAGE_PPM)
        {
            // 木有实现
        }

        return false;
    }

    float getChannelColor(int x, int y, int channel) const
    {
        x = x >= width ? x - 1 : x;
        y = y >= height ? y - 1 : y;

        if(type == A3_IMAGE_PNG)
            return image[(x + y * width) * 4 + channel] / 255.0f;
        else if(type == A3_IMAGE_EXR)
            return buffer[(x + y * width) * 4 + channel];
        else
            return 0.0f;
    }

    t3Vector3f getColor(float x, float y) const
    {
        x = x < 0.0f ? 0.0f : x;
        y = y < 0.0f ? 0.0f : y;

        float f00, f10, f01, f11;
        float color[3];

        int floorX = t3Math::floor(x), floorY = t3Math::floor(y);

        float dx = x - floorX, dy = y - floorY;

        for(int i = 0; i < 3; i++)
        {
            f00 = getChannelColor(floorX, floorY, i);
            f10 = getChannelColor(floorX + 1, floorY, i);
            f11 = getChannelColor(floorX + 1, floorY + 1, i);
            f01 = getChannelColor(floorX, floorY + 1, i);

            color[i] = a3BilinearInterpolation(x - floorX, y - floorY, f00, f01, f11, f10);
        }

        return t3Vector3f(color[0], color[1], color[2]);
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
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                if(type == A3_IMAGE_PNG)
                {
                    a3Log::print("r: %d, ", image[(i * width + j) * 4 + 0]);
                    a3Log::print("g: %d, ", image[(i * width + j) * 4 + 1]);
                    a3Log::print("b: %d, ", image[(i * width + j) * 4 + 2]);
                    a3Log::print("a: %d\n", image[(i * width + j) * 4 + 3]);
                }
                else if(type == A3_IMAGE_EXR)
                {
                    a3Log::print("r: %d, ", buffer[(i * width + j) * 4 + 0]);
                    a3Log::print("g: %d, ", buffer[(i * width + j) * 4 + 1]);
                    a3Log::print("b: %d, ", buffer[(i * width + j) * 4 + 2]);
                    a3Log::print("a: %d\n", buffer[(i * width + j) * 4 + 3]);
                }
            }
        }
    }

#ifdef A3_IMAGE_LIB_PNGPP
    png::image< png::rgb_pixel >* im;
#else
    std::vector<unsigned char> image;
#endif

    // 图片格式
    a3ImageType type;

    float* buffer;

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
    char extension[32];
    _splitpath(filePath.c_str(), NULL, NULL, NULL, extension);

    // 根据后缀解析文件格式
    if(!_stricmp(extension, ".png"))
        decoder->type = A3_IMAGE_PNG;
    else if(!_stricmp(extension, ".exr"))
        decoder->type = A3_IMAGE_EXR;
    else if(!_stricmp(extension, ".ppm"))
        decoder->type = A3_IMAGE_PPM;
    else 
        decoder->type = A3_IMAGE_ERROR;

    bLoaded = decoder->load(filePath);

    return bLoaded;
}

t3Vector3f a3ImageDecoder::getColor(int x, int y) const
{
    return decoder->getColor(x, y);
}

t3Vector3f a3ImageDecoder::lookup(float u, float v) const
{
    return decoder->getColor(u * decoder->width - 0.5f, v * decoder->height - 0.5f);
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

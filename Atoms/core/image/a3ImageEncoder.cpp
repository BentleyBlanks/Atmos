#include <core/image/a3ImageEncoder.h>

#ifdef A3_IMAGE_LIB_PNGPP
#include <png.hpp>

#elif defined A3_IMAGE_LIB_LODEPNG
#include <lodepng.h>
#endif

#ifdef A3_IMAGE_LIB_TINYEXR
#include <tinyexr.h>
#endif

// --!烂设计
bool a3SaveExr(const float* buffer, int width, int height, const char* fileName)
{
    //float* channels[3];

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 3;

    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
    const char* channel_names[] = {"B", "G", "R"}; // "B", "G", "R", "A" for RGBA image

    std::vector<float> images[3];
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    for(int i = 0; i < width * height; i++)
    {
        images[0][i] = buffer[4 * i + 0];
        images[1][i] = buffer[4 * i + 1];
        images[2][i] = buffer[4 * i + 2];
    }

    float* image_ptr[3];
    image_ptr[0] = &(images[2].at(0)); // B
    image_ptr[1] = &(images[1].at(0)); // G
    image_ptr[2] = &(images[0].at(0)); // R

    image.channel_names = channel_names;
    image.images = (unsigned char**) image_ptr;
    image.width = width;
    image.height = height;
    image.compression = TINYEXR_COMPRESSIONTYPE_ZIP;

    image.pixel_types = (int *) malloc(sizeof(int) * image.num_channels);
    image.requested_pixel_types = (int *) malloc(sizeof(int) * image.num_channels);
    for(int i = 0; i < image.num_channels; i++)
    {
        image.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
        image.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
    }

    const char* error;
    int ret = SaveMultiChannelEXRToFile(&image, fileName, &error);
    if(ret != 0)
    {
        a3Log::error("Encoder error %d: %s\n", error);
        a3Log::error("Image %s.png saved failed\n", fileName);
        return false;
    }

    a3Log::success("Image %s saved succeed\n", fileName);
    a3Log::success("width:%d, height:%d\n", width, height);

    free(image.pixel_types);
    free(image.requested_pixel_types);

    return false;
}

class a3ImageEncoder::a3Encoder
{
public:
    a3Encoder(unsigned width, unsigned height, a3ImageType type) : width(width), height(height)
    {
#ifdef A3_IMAGE_LIB_PNGPP
        im = new png::image< png::rgb_pixel >(width, height);
#else
        // 不做安全检查
        image.resize(width * height * 4);
#endif
    }

    void setColor(int x, int y, const t3Vector3f& L)
    {
#ifdef A3_IMAGE_LIB_PNGPP
        png::rgb_pixel temp = (*im)[y][x];

        //int one = L.x + temp.red, two = L.y + temp.green, three = L.z + temp.blue;
        //a3Log::debug("r:%d g:%d b:%d\n", one, two, three);

        //(*im)[y][x] = png::rgb_pixel(t3Math::Min<unsigned int>(L.x + temp.red, 255), t3Math::Min<unsigned int>(L.y + temp.green, 255), t3Math::Min<unsigned int>(L.z + temp.blue, 255));
        (*im)[y][x] = png::rgb_pixel(L.x + temp.red, L.y + temp.green, L.z + temp.blue);
#else
        image[(x + y * width) * 4 + 0] = L.x;
        image[(x + y * width) * 4 + 1] = L.y;
        image[(x + y * width) * 4 + 2] = L.z;
        image[(x + y * width) * 4 + 3] = 255.0f;
#endif
    }

    void write(std::string& fileName)
    {
#ifdef A3_IMAGE_LIBPNG
        im->write(fileName.c_str());
#else
        unsigned error = lodepng::encode(fileName, image, width, height);

        //if there's an error, display it
        if(error)
        {
            a3Log::error("Encoder error %d: %s\n", error, lodepng_error_text(error));
            a3Log::error("Image %s.png saved failed\n", fileName.c_str());
        }
        else
        {
            a3Log::success("Image %s.png saved succeed\n", fileName.c_str());
            a3Log::success("width:%d, height:%d\n", width, height);
        }
#endif
    }

#ifdef A3_IMAGE_LIB_PNGPP
    png::image< png::rgb_pixel >* im;
#else
    std::vector<unsigned char> image;
#endif

    // 重复记录 烂设计……
    unsigned width, height;
};


a3ImageEncoder::a3ImageEncoder(unsigned width, unsigned height, a3ImageType type) :type(type)
{
    encoder = new a3Encoder(width, height, type);

    if(type == A3_IMAGE_EXR)
    {
        buffer = new float[width * height * 4];
    }
    else if(type == A3_IMAGE_PPM)
    {
        // init ppm image's pixel
        pixels = new t3Vector3f*[encoder->width];
        for(int i = 0; i < encoder->width; i++)
            pixels[i] = new t3Vector3f[encoder->height];
    }
}

a3ImageEncoder::~a3ImageEncoder()
{
    for(int i = 0; i < encoder->width; i++)
        delete[] pixels[i];
    delete[] pixels;
    pixels = NULL;

    delete encoder;
    encoder = NULL;
}

void a3ImageEncoder::setColor(int x, int y, const t3Vector3f& L)
{
    switch(type)
    {
    case A3_IMAGE_PPM:
        pixels[x][y] = L * 255.0f;
        break;
    case A3_IMAGE_PNG:
        encoder->setColor(x, y, L * 255.0f);
        break;
    case A3_IMAGE_EXR:
        buffer[(y * encoder->width + x) * 4 + 0] = L.x;
        buffer[(y * encoder->width + x) * 4 + 1] = L.y;
        buffer[(y * encoder->width + x) * 4 + 2] = L.z;
        break;
    default:
        a3Log::error("图像文件格式: %d设定有误\n", type);
        break;
    }
}

void a3ImageEncoder::write(std::string& fileName)
{
    FILE* f = NULL;
    switch(type)
    {
    case A3_IMAGE_PPM:
        f = fopen((fileName).c_str(), "w");

        fprintf(f, "P3\n%d %d\n%d\n ", encoder->width, encoder->height, 255);
        for(int row = 0; row < encoder->height; row++)
        {
            for(int col = 0; col < encoder->width; col++)
                fprintf(f, "%d %d %d ", std::min((int) pixels[col][row].x, 255), std::min((int) pixels[col][row].y, 255), std::min((int) pixels[col][row].z, 255));

            fprintf(f, "\n");
        }

        a3Log::success("a3ImageEncoder::write() 文件%s 保存成功", fileName.c_str());

        fclose(f);
        break;

    case A3_IMAGE_PNG:
        encoder->write(fileName);
        break;

    case A3_IMAGE_EXR:
        a3SaveExr(buffer, encoder->width, encoder->height, fileName.c_str());
        break;

    default:
        a3Log::error("图像文件格式: %d设定有误\n", type);
    }
}
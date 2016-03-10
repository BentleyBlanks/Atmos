#include <core/image/a3ImageEncoder.h>

#ifdef A3_IMAGE_LIB_PNGPP
#include <png.hpp>

#elif defined A3_IMAGE_LIB_LODEPNG
#include <lodepng.h>
#endif

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
        unsigned error = lodepng::encode(fileName + ".png", image, width, height);

        //if there's an error, display it
        if(error)
            a3Log::error("encoder error %d: %s\n", error, lodepng_error_text(error));
#endif

        a3Log::success("Image %s.png saved succeed\n", fileName.c_str());
        a3Log::success("width:%d, height:%d\n", width, height);
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

    // init ppm image's pixel
    // may waste memory
    pixels = new t3Vector3f*[encoder->width];
    for(int i = 0; i < encoder->width; i++)
        pixels[i] = new t3Vector3f[encoder->height];
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
        pixels[x][y] = L;
        break;
    case A3_IMAGE_PNG:
        encoder->setColor(x, y, L);
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
        f = fopen((fileName + ".ppm").c_str(), "w");

        fprintf(f, "P3\n%d %d\n%d\n ", encoder->width, encoder->height, 255);
        for(int row = 0; row < encoder->height; row++)
        {
            for(int col = 0; col < encoder->width; col++)
                fprintf(f, "%d %d %d ", std::min((int) pixels[col][row].x, 255), std::min((int) pixels[col][row].y, 255), std::min((int) pixels[col][row].z, 255));

            fprintf(f, "\n");
        }

        a3Log::success("文件%s.ppm保存成功", fileName.c_str());

        fclose(f);
        break;

    case A3_IMAGE_PNG:
        encoder->write(fileName);
        break;

    default:
        a3Log::error("图像文件格式: %d设定有误\n", type);
    }
}
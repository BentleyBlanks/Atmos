#include <core/image/a3Image.h>
#include <samples/a3CameraSample.h>
#include <core/log/a3Log.h>
#include <png.hpp>

class a3Image::a3Picture
{
public:
    a3Picture(int width, int height)
    {
        im = new png::image< png::rgb_pixel >(width, height);
    }

    void add(int x, int y, const t3Vector3f& L)
    {
        png::rgb_pixel temp = (*im)[y][x];

        //int one = L.x + temp.red, two = L.y + temp.green, three = L.z + temp.blue;
        //a3Log::debug("r:%d g:%d b:%d\n", one, two, three);
        
        (*im)[y][x] = png::rgb_pixel(t3Math::Min<unsigned int>(L.x + temp.red, 255), t3Math::Min<unsigned int>(L.y + temp.green, 255), t3Math::Min<unsigned int>(L.z + temp.blue, 255));
    }

    void write(std::string& fileName)
    {
        im->write(fileName.c_str());
    }

    png::image< png::rgb_pixel >* im;
};

a3Image::a3Image(int width, int height, const std::string& fileName, a3ImageType type) : width(width), height(height), fileName(fileName), type(type)
{
	// init image's pixel
	pixels = new t3Vector3f*[width];
	for(int i = 0; i < width; i++)
		pixels[i] = new t3Vector3f[height];

    picture = new a3Picture(width, height);
}

a3Image::~a3Image()
{
	for(int i = 0; i < width; i++)
		delete[] pixels[i];
	delete[] pixels;
}

void a3Image::write()
{
    FILE *f = NULL;

    switch(type)
    {
    case A3_IMAGE_PPM:	
        f = fopen((fileName + ".ppm").c_str(), "w");

        fprintf(f, "P3\n%d %d\n%d\n ", width, height, 255);
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
                fprintf(f, "%d %d %d ", std::min((int) pixels[col][row].x, 255), std::min((int) pixels[col][row].y, 255), std::min((int) pixels[col][row].z, 255));

            fprintf(f, "\n");
        }

        a3Log::success("文件%s.ppm保存成功", fileName.c_str());

        fclose(f);
        break;

    case A3_IMAGE_PNG:
        picture->write(fileName + ".png");

        a3Log::success("文件%s.png保存成功", fileName.c_str());
        break;

    default:
        a3Log::error("图像文件格式: %d设定有误", type);
    }

}

void a3Image::addSample(const a3CameraSample* sample, const t3Vector3f& L)
{
    int x = sample->imageX, y = sample->imageY;
    switch(type)
    {
    case A3_IMAGE_PPM:
        pixels[x][y] = L;
        break;
    case A3_IMAGE_PNG:
        picture->add(x, y, L);
        break;
    default:
        a3Log::error("图像文件格式: %d设定有误", type);
        break;
    }
}
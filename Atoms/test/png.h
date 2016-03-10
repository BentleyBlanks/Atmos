#include <png.hpp>

#include <lodepng.h>

#include <vector>

#include <core/log/a3Log.h>
//#define PNG_PP

#define LOADPNG

int main()
{
#ifdef PNG_PP    
    png::image< png::rgb_pixel > image(128, 128);
    for(size_t y = 0; y < image.get_height(); ++y)
    {
        for(size_t x = 0; x < image.get_width(); ++x)
        {
            image[y][x] = png::rgb_pixel(x, y, x + y);
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    image.write("rgb.png");
#endif

#ifdef LOADPNG
    //decode
    std::vector<unsigned char> image;
    unsigned width, height;

    unsigned error = lodepng::decode(image, width, height, "test.png");

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    
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

    image[50] = 100;

    // encode
    //Encode the image
    error = lodepng::encode("test1.png", image, width, height);

    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
#endif

    return 0;
}
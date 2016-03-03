#include <png.hpp>

int main()
{
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

    return 0;
}
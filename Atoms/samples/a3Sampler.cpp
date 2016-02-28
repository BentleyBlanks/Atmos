#include <samples/a3Sampler.h>
#include <samples/a3CameraSample.h>

a3Sampler::a3Sampler(int xStart, int yStart, int xEnd, int yEnd, int spp)
: xStart(xStart), xEnd(xEnd), yStart(yStart), yEnd(yEnd), samplesPerPixel(spp)
{
    x = xStart;
    y = yStart;
}

int a3Sampler::getSample(const int x, const int y, a3CameraSample *sample)
{
    sample->imageX = x;
    sample->imageY = y;

    //if(x + 1 >= xEnd)
    //    return 0;
    //else
    //    x++;

    //if(y + 1 >= yEnd)
    //    return 0;
    //else
    //    y++;
     
    return 1;
}


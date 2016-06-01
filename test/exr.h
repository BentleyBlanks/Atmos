#pragma once
#include <iostream>
#include <tinyexr.h>
#include <vector>
#include <core/log/a3Log.h>

//#define LOADING_FROM_FILE

bool SaveEXR(const float* rgb, int width, int height, const char* outfilename)
{
    float* channels[3];

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
        images[0][i] = rgb[3 * i + 0];
        images[1][i] = rgb[3 * i + 1];
        images[2][i] = rgb[3 * i + 2];
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

    const char* err;
    int ret = SaveMultiChannelEXRToFile(&image, outfilename, &err);
    if(ret != 0)
    {
        fprintf(stderr, "Save EXR err: %s\n", err);
        return ret;
    }
    printf("Saved exr file. [ %s ] \n", outfilename);

    free(image.pixel_types);
    free(image.requested_pixel_types);

    return ret;

}

int main()
{
#ifndef LOADING_FROM_FILE
    const char* input = "../../../../resources/images/envmap.exr";
    float* out; // width * height * RGBA
    int width;
    int height;
    const char* err;

    int ret = LoadEXR(&out, &width, &height, input, &err);

    if(ret == -1)
    {
        a3Log::print("%s\n", err);
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            a3Log::print("%f ", out[i * width + j + 0]);
            a3Log::print("%f ", out[i * width + j + 1]);
            a3Log::print("%f ", out[i * width + j + 2]);
            a3Log::print("%f\n", out[i * width + j + 3]);
        }
    }
#else
    // loadingfrom file
    const char* input = "../../../../resources/imageas/envmap.exr";
    const char* err;

    EXRImage exrImage;
    InitEXRImage(&exrImage);

    int ret = ParseMultiChannelEXRHeaderFromFile(&exrImage, input, &err);
    if(ret != 0)
    {
        fprintf(stderr, "Parse EXR err: %s\n", err);
        return;
    }

    //// Uncomment if you want reading HALF image as FLOAT.
    //for (int i = 0; i < exrImage.num_channels; i++) {
    //  if (exrImage.pixel_types[i] = TINYEXR_PIXELTYPE_HALF) {
    //    exrImage.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    //  }
    //}

    ret = LoadMultiChannelEXRFromFile(&exrImage, input, &err);
    if(ret != 0)
    {
        fprintf(stderr, "Load EXR err: %s\n", err);
        return;
    }
#endif

    // Saving exr file


    getchar();
    return 0;
}
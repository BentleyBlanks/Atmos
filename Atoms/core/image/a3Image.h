#ifndef A3_IMAGE_H
#define A3_IMAGE_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>

class a3CameraSample;

enum a3ImageType
{
    A3_IMAGE_ERROR = -1,
    A3_IMAGE_PPM = 0,
    A3_IMAGE_PNG = 1
};

class a3Image
{
public:
	a3Image(int width, int height, const std::string& fileName, a3ImageType type = A3_IMAGE_PNG);

	~a3Image();

    void addSample(const a3CameraSample* sample, const t3Vector3f& L);

	void write();

	t3Vector3f **pixels;

	int width, height;

	std::string fileName;

    int type;

private:
    class a3Picture;
    a3Picture* picture;
};

#endif
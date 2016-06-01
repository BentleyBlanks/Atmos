#ifndef A3_IMAGE_H
#define A3_IMAGE_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>

class a3CameraSample;
class a3ImageEncoder;

class a3Film
{
public:
    a3Film(unsigned width, unsigned height, const std::string& fileName);

	~a3Film();

    virtual void addColor(int x, int y, const t3Vector3f& color);

    virtual void addSample(const a3CameraSample* sample, const t3Vector3f& L);

    void setFileName(const std::string& fileName);

	void write();

	int width, height;

	std::string fileName;
    
//protected:
    a3ImageEncoder* picture;
};

#endif
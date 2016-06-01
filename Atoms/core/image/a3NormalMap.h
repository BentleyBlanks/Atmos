#ifndef A3_NORMALMAP_H
#define A3_NORMALMAP_H

#include <core/a3Settings.h>
#include <t3Math/core/t3Vector3.h>
#include <core/image/a3Film.h>

class a3NormalMap : public a3Film
{
public:
    a3NormalMap(unsigned width, unsigned height, const std::string& fileName);

    a3NormalMap(const a3Film& film);

    ~a3NormalMap();

    virtual void addSample(const a3CameraSample* sample, const t3Vector3f& normal);
};

#endif
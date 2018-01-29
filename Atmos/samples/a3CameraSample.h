#pragma once

class a3CameraSample
{
public:
    a3CameraSample() :imageX(0), imageY(0) {}

    a3CameraSample(float imageX, float imageY) :imageX(imageX), imageY(imageY) {}

    float imageX, imageY;
};
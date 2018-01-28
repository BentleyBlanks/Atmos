#pragma once

// thread-safe random number generator
class a3Random
{
public:
    a3Random();

    static double randomDouble(const double min = 0.0f, const double max = 1.0f);

    static float randomFloat(const float min = 0.0f, const float max = 1.0f);

    static int randomInt(const int min = 0, const int max = 100);
};

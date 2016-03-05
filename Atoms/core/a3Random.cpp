#include <core/a3Random.h>
#include <random>

class a3Random::a3RandomPrivate
{
public:
    a3RandomPrivate() :mt(random()), mersenneTwister(random()), uniformi(0, 10), uniformf(0.0f, 1.0f)
    {
    }

    float randomFloat()
    {
        return uniformf(mersenneTwister);
    }

    int randomInt()
    {
        return uniformi(mt);
    }

    // 产生随机数种子
    std::random_device random;
    // mersenne twister engine
    std::mt19937 mt, mersenneTwister;
    // uniform distribution
    std::uniform_int_distribution<> uniformi;
    std::uniform_real_distribution<float> uniformf;
};

a3Random::a3Random()
{
    random = new a3RandomPrivate();
}

float a3Random::randomFloat() const
{
    return random->randomFloat();
}

unsigned int a3Random::randomInt() const
{
    return random->randomInt();
}

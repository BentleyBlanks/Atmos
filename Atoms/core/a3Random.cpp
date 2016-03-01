#include <core/a3Random.h>
#include <random>

// 产生随机数种子
std::random_device random;
// mersenne twister engine
std::mt19937 mt(random()), mersenneTwister(random());
// uniform distribution
std::uniform_int_distribution<> uniformi(0, 10);
std::uniform_real_distribution<float> uniformf(0.0f, 1.0f);

a3Random::a3Random()
{

}

float a3Random::randomFloat() const
{
    return uniformf(mersenneTwister);
}

unsigned int a3Random::randomInt() const
{
    return uniformi(mt);
}

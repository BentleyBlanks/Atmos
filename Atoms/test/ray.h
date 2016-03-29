#include <shapes/a3Sphere.h>
#include <core/image/a3Film.h>
#include <core/a3Ray.h>
#include <core/log/a3Log.h>
#include <core/log/a3LogFile.h>
#include <core/log/a3LogHTML.h>
#include <random>
#include <core/a3Random.h>

int main()
{
    a3Random random;
    for(size_t i = 0; i < 20; i++)
        a3Log::debug("%f\n", random.randomFloat());

    for(size_t i = 0; i < 20; i++)
        a3Log::debug("%d\n", random.randomInt());

	a3Sphere sphere(t3Vector3f(0, 10, 0), 10);

	sphere.getNormal(t3Vector3f(20, 10, 0)).print();

	a3Film image(900, 900, "Atoms.ppm");


	// 产生随机数种子
	std::random_device randoms;
	// mersenne twister engine
	std::mt19937 mt(randoms()), mersenneTwister(randoms());
	// uniform distribution
	std::uniform_int_distribution<> uniformi(1, 6);
	std::uniform_real_distribution<float> uniformf(0, 1);
	// normal distribution
	std::normal_distribution<> d(5, 2);

#define RND (2.0*uniform(mersenneTwister)-1.0)
#define RND2 (uniform(mersenneTwister))

#define A3RANDOM uniformf(mersenneTwister)

	for(int i = 0; i < 10; i++)
		std::cout << mt() << std::endl;

	for(int i = 0; i < 10; i++)
		std::cout << uniformi(mt) << std::endl;

	for(int i = 0; i < 10; i++)
		std::cout << uniformf(mersenneTwister) << std::endl;

	for(int i = 0; i < image.width; i++)
	{
		for(int j = 0; j < image.height; j++)
		{
            // deprecated
            //image.pixels[i][j].set(A3RANDOM, A3RANDOM, A3RANDOM);
		}
	}

	image.write();
	
    a3Ray ray(t3Vector3f(10, 0, 0), t3Vector3f(-1, 0, 0), 0.0f, 10000.0f);
    a3Sphere sphere1(t3Vector3f(0, 0, 0), 1.0f);

    ray.print();
    sphere1.print();
    float t;
    sphere1.intersect(ray, &t);
    std::cout << "相交测试最近点为: " << t << std::endl;

    // log test
    int a = 10;
    a3Log::error("这一段是错误Log, 紧接着一个int变量: a = %d\n", a);
    a3Log::warning("这一段是警告Log, 紧接着一个int变量: a = %d\n", a);
    a3Log::success("这一段是成功Log, 紧接着一个int变量: a = %d\n", a);
    a3Log::info("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);
    a3Log::debug("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);
    a3LogFile::debug("What are you talking about%d\n", a);

    a3LogFile::log(A3_LOG_LEVEL_DEBUG, "WTF", "log", "This is a File Log. a = %d\n", a);
    
    //a3LogFile::log(A3_LOG_LEVEL_DEBUG, "WTF", "log", "AAAAAAAAAAAAA. a = %d\n", a);
    //a3LogFile::log(A3_LOG_LEVEL_DEBUG, "WTF", "log", "AAAAAAAAAAAAA. a = %d\n", a);

    a3LogFile::error("This is a File Log. a = %d\n", a);
    a3LogFile::warning("This is a File Log. a = %d\n", a);
    a3LogFile::success("This is a File Log. a = %d\n", a);
    a3LogFile::info("This is a File Log. a = %d\n", a);
    a3LogFile::debug("This is a File Log. a = %d\n", a);

    //t2Log("这一段是信息Log, 紧接着一个int变量: a = %d\n", a);

    a3LogHTML::begin();
    a3LogHTML::error("This is a File Log. a = %d\n", a);
    a3LogHTML::warning("This is a File Log. a = %d\n", a);
    a3LogHTML::success("This is a File Log. a = %d\n", a);
    a3LogHTML::info("This is a File Log. a = %d\n", a);
    a3LogHTML::debug("This is a File Log. a = %d\n", a);
    a3LogHTML::end();

	getchar();

	return 0;
}
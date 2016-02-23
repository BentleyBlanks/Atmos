#include <shapes/a3Sphere.h>
#include <film/a3Image.h>
#include <core/a3Ray.h>
#include <core/a3Log.h>
#include <random>

int main()
{
	a3Sphere sphere(t3Vector3f(0, 10, 0), 10);

	sphere.normal(t3Vector3f(20, 10, 0)).print();

	a3Image image(900, 900, "Atoms.ppm");


	// �������������
	std::random_device random;
	// mersenne twister engine
	std::mt19937 mt(random()), mersenneTwister(random());
	// uniform distribution
	std::uniform_int_distribution<> uniformi(1, 6);
	std::uniform_real_distribution<float> uniformf(0, 255);
	// normal distribution
	std::normal_distribution<> d(5, 2);

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
            image.pixels[i][j].set(A3RANDOM, A3RANDOM, A3RANDOM);
		}
	}

	image.write();
	
    a3Ray ray(t3Vector3f(10, 0, 0), t3Vector3f(-1, 0, 0), 0.0f, 10000.0f);
    a3Sphere sphere1(t3Vector3f(0, 0, 0), 1.0f);

    ray.print();
    sphere1.print();
    std::cout << "�ཻ���������Ϊ: " << sphere1.intersect(ray) << std::endl;

    // log test
    int a = 10;
    a3Log::error("��һ���Ǵ���Log, ������һ��int����: a = %d\n", a);
    a3Log::warning("��һ���Ǿ���Log, ������һ��int����: a = %d\n", a);
    a3Log::success("��һ���ǳɹ�Log, ������һ��int����: a = %d\n", a);
    a3Log::info("��һ������ϢLog, ������һ��int����: a = %d\n", a);
    a3Log::debug("��һ������ϢLog, ������һ��int����: a = %d\n", a);
    a3Log::debug("WTF", "log", "What are you talking about%d\n", a);

    a3Log::log(A3_LOG_LEVEL_DEBUG, "WTF", "log", "This is a File Log. a = %d\n", a);
    
    a3Log::log(A3_LOG_LEVEL_DEBUG, "WTF", "log", "AAAAAAAAAAAAA. a = %d\n", a);
    a3Log::log(A3_LOG_LEVEL_DEBUG, "WTF", "log", "AAAAAAAAAAAAA. a = %d\n", a);

    aa3Log("��һ������ϢLog, ������һ��int����: a = %d\n", a);

	getchar();

	return 0;
}
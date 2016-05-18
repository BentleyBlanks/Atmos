#include <renderers/a3SamplerRenderer.h>
#include <cameras/a3PerspectiveSensor.h>
#include <core/image/a3Film.h>
#include <core/a3Scene.h>
#include <shapes/a3Sphere.h>
#include <core/log/a3Log.h>
#include <shapes/a3Plane.h>
#include <shapes/a3Disk.h>
#include <shapes/a3Triangle.h>
#include <Common/t3Timer.h>
#include <core/a3ModelImporter.h>
#include <lights/a3InfiniteAreaLight.h>
#include <core/a3Common.h>
#include <core/log/a3LogFile.h>

int main()
{
	a3LogFile::debug("-------------------------------------------------------------\n");

	a3Film* image = new a3Film(1000, 1000, "hello", A3_IMAGE_PNG);
	a3Film* image2 = new a3Film(3200, 3200, "hello", A3_IMAGE_PNG);

	a3PerspectiveSensor* camera = new a3PerspectiveSensor(t3Vector3f(0, 100, 0), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 1.0f, 2, 2 * image->width / image->height, 1.0f, 210.0f, 0.0f, image);
	a3PerspectiveSensor* camera2 = new a3PerspectiveSensor(t3Vector3f(0, 100, 0), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1), 1.0f, 3.8, 3.8 * image->width / image->height, 1.0f, 210.0f, 0.0f, image2);

	a3Log::debug("fov: %f, %f\n", t3Math::Rad2Deg(camera->fov.x), t3Math::Rad2Deg(camera->fov.y));
	a3Log::debug("focal distance: %f, lens radius: %f\n", camera->focalDistance, camera->lensRadius);
	a3LogFile::debug("fov: %f, %f\n", t3Math::Rad2Deg(camera->fov.x), t3Math::Rad2Deg(camera->fov.y));
	a3LogFile::debug("focal distance: %f, lens radius: %f\n", camera->focalDistance, camera->lensRadius);

	// 场景列表
	a3Scene* scene = new a3Scene();
	a3Scene* scene2 = new a3Scene();
	a3Scene* scene3 = new a3Scene();

	auto addShape = [&scene](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
	{
		s->emission = emission;
		s->color = color;
		s->type = type;
		scene->addShape(s);
	};

	auto addShape2 = [&scene2](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
	{
		s->emission = emission;
		s->color = color;
		s->type = type;
		scene2->addShape(s);
	};

	auto addShape3 = [&scene3](a3Shape* s, t3Vector3f color, t3Vector3f emission, int type)
	{
		s->emission = emission;
		s->color = color;
		s->type = type;
		scene3->addShape(s);
	};

	a3SamplerRenderer* renderer = new a3SamplerRenderer();

	// queue start
	t3Timer timer;

	// -----------------------------Scene Cornel Box-----------------------------
	a3Log::debug("Scene Cornel Box Started\n");
	a3LogFile::debug("Scene Cornel Box Started\n");

	// Sphere/Disk    
	addShape2(new a3Sphere(t3Vector3f(-30, -74, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), MIRROR);
	addShape2(new a3Sphere(t3Vector3f(0, 0, 90), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), MIRROR);
	addShape2(new a3Disk(t3Vector3f(30, 74, 80), 60, t3Vector3f(0, 0, -1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape2(new a3Sphere(t3Vector3f(-30, 74, 70), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), GLASS);
	addShape2(new a3Sphere(t3Vector3f(30, -59, 60), 30), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), GLASS);

	// Plane
	addShape2(new a3Plane(t3Vector3f(-70, 0, 100), t3Vector3f(-1, 0, 0)), t3Vector3f(7, 2, 2), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape2(new a3Plane(t3Vector3f(70, 0, 100), t3Vector3f(1, 0, 0)), t3Vector3f(2, 2, 7), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape2(new a3Plane(t3Vector3f(0, 0, 140), t3Vector3f(0, 0, 1)), t3Vector3f(7, 7, 9), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape2(new a3Plane(t3Vector3f(0, 90, 0), t3Vector3f(0, 1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape2(new a3Plane(t3Vector3f(0, -90, 0), t3Vector3f(0, -1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), LAMBERTIAN);

	// 光源 自发光
	addShape2(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(160, 40, 40), LAMBERTIAN);
	addShape2(new a3Disk(t3Vector3f(69, 0, 100), 100, t3Vector3f(1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(40, 160, 40), LAMBERTIAN);
	addShape2(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(140, 40, 40), LAMBERTIAN);

	camera2->setCameraToWorld(t3Vector3f(0, 0, -120), t3Vector3f(0, 0, 0), t3Vector3f(0, 1, 0));
	renderer->camera = camera2;
	image2->setFileName("Cornel Box/1");

	timer.start();
	renderer->render(scene2);
	timer.end();

	a3Log::debug("Scene Cornel Box Ended\n");
	a3LogFile::debug("Scene Cornel Box Ended\n");

	a3Log::info("Cornel Box Cost time: %f sec\n", timer.difference());
	a3LogFile::info("Cornel Box Cost time: %f sec\n", timer.difference());



	// -----------------------------Scene Cornel Box With Depth of field-----------------------------
	a3Log::debug("Scene Cornel Box With Depth of field Started\n");
	a3LogFile::debug("Scene Cornel Box With Depth of field Started\n");



	// Sphere/Disk    
	addShape3(new a3Sphere(t3Vector3f(-30, -74, 100), 15), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), MIRROR);
	addShape3(new a3Sphere(t3Vector3f(0, 0, 90), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), MIRROR);
	addShape3(new a3Disk(t3Vector3f(30, 74, 80), 60, t3Vector3f(0, 0, -1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape3(new a3Sphere(t3Vector3f(-30, 74, 70), 15), t3Vector3f(9, 9, 9), t3Vector3f(0, 0, 0), GLASS);
	addShape3(new a3Sphere(t3Vector3f(30, -59, 60), 30), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), GLASS);

	// Plane
	addShape3(new a3Plane(t3Vector3f(-70, 0, 100), t3Vector3f(-1, 0, 0)), t3Vector3f(7, 2, 2), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape3(new a3Plane(t3Vector3f(70, 0, 100), t3Vector3f(1, 0, 0)), t3Vector3f(2, 2, 7), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape3(new a3Plane(t3Vector3f(0, 0, 140), t3Vector3f(0, 0, 1)), t3Vector3f(7, 7, 9), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape3(new a3Plane(t3Vector3f(0, 90, 0), t3Vector3f(0, 1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), LAMBERTIAN);
	addShape3(new a3Plane(t3Vector3f(0, -90, 0), t3Vector3f(0, -1, 0)), t3Vector3f(7, 7, 7), t3Vector3f(0, 0, 0), LAMBERTIAN);

	// 光源 自发光
	addShape3(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(160, 40, 40), LAMBERTIAN);
	addShape3(new a3Disk(t3Vector3f(69, 0, 100), 100, t3Vector3f(1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(40, 160, 40), LAMBERTIAN);
	addShape3(new a3Disk(t3Vector3f(-69, 0, 100), 100, t3Vector3f(-1, 0, 0)), t3Vector3f(0, 0, 0), t3Vector3f(140, 40, 40), LAMBERTIAN);

	camera2->setCameraToWorld(t3Vector3f(0, 0, -120), t3Vector3f(0, 0, 0), t3Vector3f(0, 1, 0));
	camera2->focalDistance = 180.0f;
	camera2->lensRadius = 18.0f;
	image2->setFileName("Cornel Box With Depth of field/1");

	timer.start();
	renderer->render(scene3);
	timer.end();

	a3Log::debug("Scene Cornel Box With Depth of field Ended\n");
	a3LogFile::debug("Scene Cornel Box With Depth of field Ended\n");

	a3Log::info("Scene Cornel Box With Depth of field Cost time: %f sec\n", timer.difference());
	a3LogFile::info("Scene Cornel Box With Depth of field Cost time: %f sec\n", timer.difference());


	// -----------------------------Scene Frame Animation-----------------------------
	a3Log::debug("Scene Frame Animation Started\n");
	a3LogFile::debug("Scene Frame Animation Started\n");

	addShape(new a3Sphere(t3Vector3f(-0, -0, 0), 35), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), MIRROR);
	addShape(new a3Disk(t3Vector3f(0, 0, -35), 60, t3Vector3f(0, 0, -1)), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 0), MIRROR);

	scene->addLight(new a3InfiniteAreaLight("1.png"));
	renderer->camera = camera;

	timer.start();

	for (int i = -100; i < 100; i += 2)
	{
		camera->setCameraToWorld(t3Vector3f(i, 100, 0), t3Vector3f(0, 0, 0), t3Vector3f(0, 0, 1));

		image->setFileName("Frame Animation/" + a3ToString(i + 100));

		renderer->render(scene);
	}	
	
	timer.end();

	a3Log::debug("Scene Frame Animation Ended\n");
	a3LogFile::debug("Scene Frame Animation Ended\n");

	a3Log::info("Scene Frame Animation Cost time: %f sec\n", timer.difference());
	a3LogFile::info("Scene Frame Animation Cost time: %f sec\n", timer.difference());

	a3LogFile::debug("-------------------------------------------------------------\n");

	getchar();
	return 0;
} 
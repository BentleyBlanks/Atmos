#include <film/a3Image.h>

a3Image::a3Image(int width, int height, const std::string& fileName) : width(width), height(height), fileName(fileName)
{
	// init image's pixel
	pixels = new t3Vector3f*[width];
	for(int i = 0; i < width; i++)
		pixels[i] = new t3Vector3f[height];
}

a3Image::~a3Image()
{
	for(int i = 0; i < width; i++)
		delete[] pixels[i];
	delete[] pixels;
}

void a3Image::write()
{
	FILE *f = fopen(fileName.c_str(), "w");
	fprintf(f, "P3\n%d %d\n%d\n ", width, height, 255);
	for(int row = 0; row<height; row++)
	{
		for(int col = 0; col<width; col++)
			fprintf(f, "%d %d %d ", std::min((int)pixels[col][row].x, 255), std::min((int)pixels[col][row].y, 255), std::min((int)pixels[col][row].z, 255));

		fprintf(f, "\n");
	}
	fclose(f);
}
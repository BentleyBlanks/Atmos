#include <iostream>
#include <t3Math/core/t3Vector2.h>
#include <t3Math/core/t3Vector3.h>
#include <t3Math/core/t3Vector4.h>

// withnot using ant unit test lib
using namespace std;

int main()
{
	t3Vector3f a(10, 20, 30), b(0, 40, 50);

	t3Vector3f c(5);

	c.print("c");

	std::cout << c[0] << c[1] << c[2] << std::endl;

	a.set(c);
	a.set(10);
	a.set(10, 20, 30);

	a.print("a");

	a.set(c);
	
	bool k = (a == c), j = (a != c), l = a.match(c);

	c = a + b;
	c += a + b;
	c = a - b;
	c -= a;

	c = a * b;
	c *= a;
	b.x = 10;
	c = a / b;
	c /= b;
	c = -c;

	c = a + 0.5;
	c = 0.5 + a;
	c += 10;
	c = b - 0.5;
	c = 10 - b;
	c -= 0.5;
	c = a * 10;
	c = 10 * a;
	c *= 2;
	c = a / 2;
	c /= 2;

	std::cout << "c's length: " << c.length() << ", c's lengthSquared: " << c.lengthSquared() << std::endl;

	c = a.getScaled(5);

	std::cout << "c's length: " << c.length() << std::endl;

	c.scale(10);

	std::cout << "c's length: " << c.length() << std::endl;

	std::cout << "distance: " << c.distance(a) << ", distanceSquared: " << c.squareDistance(a) << std::endl;

	t3Vector3f d = c.getInterpolated(a, 0.5);
	c.interpolate(a, 0.5);
	d = c.getMiddle(a);
	c.middle(a);
	
	t3Vector3f e[3];
	d = d.average(e, 3);

	b = a.getNormalized();
	a.normalize();

	b = c.getLimited(10);
	b.limit(10);

	t3Vector3f f(1, 0, 0), g(0, 1, 0);
	c = f.getCrossed(g);
	c = f.cross(g);
	c = f.getPerpendicular(g);
	c = f.perpendicular(g);

	std::cout << "dot: " << a.dot(b) << std::endl;

	f = t3Vector3f::one();
	f = t3Vector3f::zero();

	getchar();

	return 0;
}
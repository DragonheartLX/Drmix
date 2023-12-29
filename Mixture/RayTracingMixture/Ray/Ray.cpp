#include "Ray/Ray.hpp"

Ray::Ray()
{

}

Ray::Ray(const Vec3& origin, const Vec3& direction):
	orig(origin),
	dir(direction)
{

}

Ray::~Ray()
{

}

Vec3 Ray::origin() const
{
	return orig;
}

Vec3 Ray::direction() const
{
	return dir;
}

Vec3 Ray::at(double t) const
{
	return orig + t * dir;
}

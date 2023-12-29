#pragma once
#include "Math/Vec3.hpp"

class Ray
{
public:
	Ray();
	Ray(const Vec3& origin, const Vec3& direction);
	~Ray();

	Vec3 origin() const;
	Vec3 direction() const;

	Vec3 at(double t) const;

private:
	Vec3 orig;
	Vec3 dir;
};
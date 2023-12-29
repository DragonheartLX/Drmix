#pragma once
#include "Object/Object.hpp"

class Sphere: public Object
{
public:
	Sphere(Vec3 center, double radius);
	
	bool hit(const Ray& r, double ray_t_Min, double ray_t_Max, HitRecord& rec) const override;
private:
	Vec3 m_Center;
	double m_Radius;
};
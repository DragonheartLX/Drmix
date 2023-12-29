#pragma once
#include "Ray/Ray.hpp"

class HitRecord
{
public:
	Vec3 p;
	Vec3 normal;
	double t;
	bool frontFace;

	void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
		// Sets the hit record normal vector.
		// NOTE: the parameter `outward_normal` is assumed to have unit length.

		frontFace = dot(r.direction(), outward_normal) < 0;
		normal = frontFace ? outward_normal : -outward_normal;
	}
};

#include <memory>
#include <vector>

class Object
{
public:
	virtual ~Object() = default;

	virtual bool hit(const Ray& r, double ray_t_Min, double ray_t_Max, HitRecord& rec) const = 0;
};

class ObjectList: public Object
{
public:
	ObjectList() {};
	ObjectList(std::shared_ptr<Object> object) { add(object); };

	void clear() { objects.clear(); };
	void add(std::shared_ptr<Object> object) { objects.push_back(object); };

	bool hit(const Ray& r, double ray_t_Min, double ray_t_Max, HitRecord& rec) const override
	{
		HitRecord tempRec;
		bool isHit = false;
		auto closestSoFar = ray_t_Max;

		for (const std::shared_ptr<Object>& object : objects) {
			if (object->hit(r, ray_t_Min, closestSoFar, tempRec)) {
				isHit = true;
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}

		return isHit;
	};

	std::vector<std::shared_ptr<Object>> objects;
private:

};

#include "Object/Sphere.hpp"
#include "Sphere.hpp"

#include "Object/Sphere.hpp"

Sphere::Sphere(Vec3 center, double radius):
	m_Center(center),
	m_Radius(radius)
{

}

bool Sphere::hit(const Ray& r, double ray_t_Min, double ray_t_Max, HitRecord& rec) const
{
    Vec3 oc = r.origin() - m_Center;
    double a = r.direction().lengthSquared();
    double half_b = dot(oc, r.direction());
    double c = oc.lengthSquared() - m_Radius * m_Radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root <= ray_t_Min || ray_t_Max <= root) {
        root = (-half_b + sqrtd) / a;
        if (root <= ray_t_Min || ray_t_Max <= root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outwardNormal = (rec.p - m_Center) / m_Radius;
    rec.setFaceNormal(r, outwardNormal);

    return true;
}

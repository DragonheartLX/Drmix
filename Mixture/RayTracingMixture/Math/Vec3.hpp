#pragma once

namespace Drmix
{
	class Vec3
	{
	public:
		Vec3();
		Vec3(double e0, double e1, double e2);

		double x() const;
		double y() const;
		double z() const;

		Vec3 operator-() const;
		double operator[](int i) const;
		double& operator[](int i);

		Vec3& operator+=(const Vec3& v);
		Vec3& operator*=(double t);
		Vec3& operator/=(double t);

		double length() const;

		double lengthSquared() const;

		double e[3];
	private:

	};

	Vec3 operator+(const Vec3& u, const Vec3& v);
	Vec3 operator-(const Vec3& u, const Vec3& v);
	Vec3 operator*(const Vec3& u, const Vec3& v);

	Vec3 operator*(double t, const Vec3& v);
	Vec3 operator*(const Vec3& v, double t);

	Vec3 operator/(Vec3 v, double t);

	double dot(const Vec3& u, const Vec3& v);
	Vec3 cross(const Vec3& u, const Vec3& v);
	Vec3 unit_vector(Vec3 v);
}
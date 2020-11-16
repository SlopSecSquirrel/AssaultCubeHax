#pragma once
#include "pch.h"


class Vec2
{
public:

	Vec2()
	{
		this->x = 0;
		this->y = 0;
	};

	Vec2(float px, float py)
	{
		this->x = px;
		this->y = py;
	};

	Vec2 sub(Vec2 v)
	{
		Vec2 p = Vec2(this->x, this->y);
		p.x -= v.x;
		p.y -= v.y;
		return p;
	};

	Vec2 mul(Vec2 v)
	{
		return Vec2(x * v.x, y * v.y);
	};

	Vec2 mul(float f)
	{
		return Vec2(x * f, y * f);
	};

	Vec2 div(Vec2 v)
	{
		return Vec2(x / v.x, y / v.y);
	};

	Vec2 operator + (const Vec2& v) const
	{
		return Vec2(x + v.x, y + v.y );
	}

	Vec2 operator / (const float& div) const
	{
		return Vec2(x / div, y / div);
	}

	Vec2 operator - (const Vec2& v) const
	{
		return Vec2(x - v.x, y - v.y);
	}

	float Length()
	{
		float flLengthX, flLengthY, flLength;

		flLengthX = x * x;
		flLengthY = y * y;

		flLength = sqrt(flLengthX + flLengthY);

		return fabs(flLength);
	}

	void Normalize()
	{
		float length, ilength;

		length = this->Length();

		if (length)
		{
			ilength = 1 / length;

			this->x *= ilength;
			this->y *= ilength; 
		}
	}

	Vec2& div(float f)
	{
		x /= f;
		y /= f; 
		return *this;
	}

	float x;
	float y;
};

class Vec3
{
public:

	Vec3()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	};

	Vec3(float px, float py, float pz)
	{
		this->x = px;
		this->y = py;
		this->z = pz;
	};

	Vec3 sub(Vec3 v)
	{
		Vec3 p = Vec3(this->x, this->y, this->z);
		p.x -= v.x;
		p.y -= v.y;
		p.z -= v.z;
		return p;
	};

	float dot(Vec3 vec)
	{
		float temp = 0;
		temp = this->x * vec.x + this->y * vec.y + this->z * vec.z;
		return temp;
	};

	float dist(Vec3 vec)
	{
		return sqrt((this->x - vec.x) * (this->x - vec.x) + (this->y - vec.y) * (this->y - vec.y) + (this->z - vec.z) * (this->z - vec.z));
	}

	Vec3 mul(Vec3 v)
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	};

	Vec3 mul(float f)
	{
		return Vec3(x * f, y * f, z * f);
	};

	Vec3 div(Vec3 v)
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	};

	Vec3 operator + (const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator / (const float& div) const
	{
		return Vec3(x / div, y / div, z / div);
	}

	Vec3 operator - (const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	float Length()
	{
		float flLengthX, flLengthY, flLengthZ, flLength;

		flLengthX = x * x;
		flLengthY = y * y;
		flLengthZ = z * z;

		flLength = sqrt(flLengthX + flLengthY + flLengthZ);

		return fabs(flLength);
	}

	void Normalize()
	{
		float length, ilength;

		length = this->Length();

		if (length)
		{
			ilength = 1 / length;

			this->x *= ilength;
			this->y *= ilength;
			this->z *= ilength;
		}
	}

	Vec3& div(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	float x;
	float y;
	float z;
};

class Vec4
{
public:

	Vec4()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	};

	Vec4(float px, float py, float pz, float pw)
	{
		this->x = px;
		this->y = py;
		this->z = pz;
		this->w = pw;
	};

	float Length()
	{
		float flLengthX, flLengthY, flLengthZ, flLengthW, flLength;

		flLengthX = x * x;
		flLengthY = y * y;
		flLengthZ = z * z;
		flLengthW = w * w;

		flLength = sqrt(flLengthX + flLengthY + flLengthZ + flLengthW);

		return fabs(flLength);
	}

	void Normalize()
	{
		float length, ilength;

		length = this->Length();

		if (length)
		{
			ilength = 1 / length;

			this->x *= ilength;
			this->y *= ilength;
			this->z *= ilength;
			this->w *= ilength;
		}
	}

	Vec4& div(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	float x;
	float y;
	float z;
	float w;
};

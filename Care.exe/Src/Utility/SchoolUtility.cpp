#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "SchoolUtility.h"

bool SchoolUtility::IsPointInRect(const Vector2<float>& point, const Vector2<float>& leftTop, const Vector2<float>& size)
{
	return point.x >= leftTop.x && point.x <= leftTop.x + size.x &&
		point.y >= leftTop.y && point.y <= leftTop.y + size.y;
}

int SchoolUtility::Clamp(int v, int min, int max)
{
	if (v > max)
	{
		v = max;
	}
	if (v < min)
	{
		v = min;
	}

	return v;
}

float SchoolUtility::Clamp(float v, int min, int max)
{
	return Clamp(v, static_cast<float>(min), static_cast<float>(max));
}

float SchoolUtility::Clamp(float v, float min, float max)
{

	if (v > max)
	{
		v = max;
	}
	if (v < min)
	{
		v = min;
	}

	return v;
}

int SchoolUtility::Round(float v)
{
	return static_cast<int>(roundf(v));
}

double SchoolUtility::Rad2DegD(double rad)
{
	return rad * (180.0 / DX_PI);
}

float SchoolUtility::Rad2DegF(float rad)
{
	return rad * (180.0f / DX_PI_F);
}

int SchoolUtility::Rad2DegI(int rad)
{
	return rad * Round(180.0f / DX_PI_F);
}

double SchoolUtility::Deg2RadD(double deg)
{
	return deg * (DX_PI / 180.0);
}

float SchoolUtility::Deg2RadF(float deg)
{
	return deg * (DX_PI_F / 180.0f);
}

int SchoolUtility::Deg2RadI(int deg)
{
	return deg * Round(DX_PI_F / 180.0f);
}

double SchoolUtility::DegIn360(double deg)
{
	deg = fmod(deg, 360.0);
	if (deg < 0.0f)
	{
		deg += 360.0;
	}
	return deg;
}

double SchoolUtility::RadIn2PI(double rad)
{
	rad = fmod(rad, DX_TWO_PI);
	if (rad < 0.0)
	{
		rad += DX_TWO_PI;
	}
	return rad;
}

int SchoolUtility::DirNearAroundRad(float from, float to)
{

	float ret = 1.0f;

	float diff = to - from;

	if (diff >= 0.0f)
	{

		// 比較元よりも時計回りに位置する

		if (diff > DX_PI_F)
		{
			// でも、180度以上離れているので、反時計回りの方が近い
			ret = -1.0f;
		}
		else
		{
			// 時計回り
			ret = 1.0f;
		}

	}
	else
	{

		// 比較元よりも反時計回りに位置する

		if (diff < -DX_PI_F)
		{
			// でも、180度以上離れているので、時計回りの方が近い
			ret = 1.0f;
		}
		else
		{
			// 反時計回り
			ret = -1.0f;
		}

	}

	return static_cast<int>(ret);

}

int SchoolUtility::DirNearAroundDeg(float from, float to)
{

	float ret = 1.0f;

	float diff = to - from;

	if (diff >= 0.0f)
	{

		// 比較元よりも時計回りに位置する

		if (diff > 180.0f)
		{
			// でも、180度以上離れているので、反時計回りの方が近い
			ret = -1.0f;
		}
		else
		{
			// 時計回り
			ret = 1.0f;
		}

	}
	else
	{

		// 比較元よりも反時計回りに位置する

		if (diff < -180.0f)
		{
			// でも、180度以上離れているので、時計回りの方が近い
			ret = 1.0f;
		}
		else
		{
			// 反時計回り
			ret = -1.0f;
		}

	}

	return static_cast<int>(ret);

}

int SchoolUtility::Lerp(int start, int end, float t)
{
	// 線形補間
	if (t >= 1.0f)
	{
		return end;
	}

	int ret = start;
	ret += Round(t * static_cast<float>(end - start));
	return ret;
}

float SchoolUtility::Lerp(float start, float end, float t)
{
	// 線形補間
	if (t >= 1.0f)
	{
		return end;
	}

	float ret = start;
	ret += t * (end - start);
	return ret;
}

double SchoolUtility::Lerp(double start, double end, double t)
{
	// 線形補間
	if (t >= 1.0)
	{
		return end;
	}

	double ret = start;
	ret += t * (end - start);
	return ret;
}

Vector2<float> SchoolUtility::Lerp(const Vector2<float>& start, const Vector2<float>& end, float t)
{
	// 線形補間
	if (t >= 1.0f)
	{
		return end;
	}

	Vector2<float> ret = start;
	ret.x += t * (end.x - start.x);
	ret.y += t * (end.y - start.y);
	return ret;
}

double SchoolUtility::LerpDeg(double start, double end, double t)
{

	double ret;

	double diff = end - start;
	if (diff < -180.0)
	{
		end += 360.0;
		ret = Lerp(start, end, t);
		if (ret >= 360.0)
		{
			ret -= 360.0;
		}
	}
	else if (diff > 180.0)
	{
		end -= 360.0;
		ret = Lerp(start, end, t);
		if (ret < 0.0)
		{
			ret += 360.0;
		}
	}
	else
	{
		ret = Lerp(start, end, t);
	}

	return ret;

}

COLOR_F SchoolUtility::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
	// 線形補間
	if (t >= 1.0f)
	{
		return end;
	}

	COLOR_F ret = start;
	ret.r += t * (end.r - start.r);
	ret.g += t * (end.g - start.g);
	ret.b += t * (end.b - start.b);
	ret.a += t * (end.a - start.a);
	return ret;
}

Vector2<float> SchoolUtility::Bezier(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, float t)
{
	Vector2<float> a = Lerp(p1, p2, t);
	Vector2<float> b = Lerp(p2, p3, t);
	return Lerp(a, b, t);
}

double SchoolUtility::Magnitude(const Vector2<float>& v)
{
	return sqrt((v.x * v.x) + (v.y * v.y));
}

float SchoolUtility::SqrMagnitude(const Vector2<float>& v)
{
	return v.x * v.x + v.y * v.y;
}

double SchoolUtility::Distance(const Vector2<float>& v1, const Vector2<float>& v2)
{
	return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

std::vector<std::string> SchoolUtility::Split(std::string& line, char delimiter)
{
	std::istringstream stream(line);
	std::string field;
	std::vector<std::string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}
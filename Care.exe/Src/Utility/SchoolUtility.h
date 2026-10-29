#pragma once
#include <vector>
#include <string>
#include <DxLib.h>
#include "../Common/Vector2.h"
class SchoolUtility
{
public:

	// デバッグ方向線の既定長
	static constexpr float DEFAULT_DEBUG_LINE_LENGTH = 50.0f;

	// ラジアンから度への換算倍率（未使用）
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);

	// 度からラジアンへの換算倍率（未使用）
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	// ベクトル間角度の算出でゼロ扱いする分母の閾値
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	// 点が左上基準の矩形内にあるか判定
	static bool IsPointInRect(const Vector2<float>& point, const Vector2<float>& leftTop, const Vector2<float>& size);

	// 値を範囲に収める
	static int Clamp(int v, int min, int max);

	// 整数の上下限による浮動小数値の制限
	static float Clamp(float v, int min, int max);

	// 浮動小数の上下限による値の制限
	static float Clamp(float v, float min, float max);

	// 四捨五入
	static int Round(float v);

	// ラジアンから度へのdouble型変換（未使用）
	static double Rad2DegD(double rad);

	// ラジアンから度へのfloat型変換（未使用）
	static float Rad2DegF(float rad);

	// ラジアンから度への整数型変換（未使用）
	static int Rad2DegI(int rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);

	// 度からラジアンへのfloat型変換（未使用）
	static float Deg2RadF(float deg);

	// 度からラジアンへの整数型変換（未使用）
	static int Deg2RadI(int deg);

	// 角度の0から360度の範囲への正規化（未使用）
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// ラジアン指定の最短回転方向の取得（未使用）
	static int DirNearAroundRad(float from, float to);

	// 度指定の最短回転方向の取得（未使用）
	static int DirNearAroundDeg(float from, float to);
	
	// 線形補間
	static int Lerp(int start, int end, float t);

	// float型の線形補間（未使用）
	static float Lerp(float start, float end, float t);

	// double型の線形補間
	static double Lerp(double start, double end, double t);

	// 整数二次元座標の線形補間
	static Vector2<float> Lerp(const Vector2<float>& start, const Vector2<float>& end, float t);

	// 度指定の角度の最短方向の線形補間（未使用）
	static double LerpDeg(double start, double end, double t);

	// 色成分の線形補間（未使用）
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// 整数二次元座標の二次ベジェ補間（未使用）
	static Vector2<float> Bezier(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, float t);

	// ベクトルの長さ
	static double Magnitude(const Vector2<float>& v);

	// 整数二次元ベクトルの長さの二乗の取得（未使用）
	static float SqrMagnitude(const Vector2<float>& v);

	// 整数二次元座標間の距離の取得（未使用）
	static double Distance(const Vector2<float>& v1, const Vector2<float>& v2);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);
};

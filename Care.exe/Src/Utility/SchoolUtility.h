#pragma once
#include <vector>
#include <string>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/Quaternion.h"
class SchoolUtility
{
public:

	// デバッグ方向線の既定長
	static constexpr float DEFAULT_DEBUG_LINE_LENGTH = 50.0f;

	// ラジアンから度への換算倍率（未使用）
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);

	// 度からラジアンへの換算倍率（未使用）
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	// 全成分がゼロのベクトル
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };

	// 全成分が1のベクトル（未使用）
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// X軸の単位ベクトル（未使用）
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };

	// Y軸の単位ベクトル（未使用）
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };

	// Z軸の単位ベクトル（未使用）
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };

	// 後方向の単位ベクトル
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };

	// 右方向の単位ベクトル
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };

	// 左方向の単位ベクトル
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };

	// 上方向の単位ベクトル
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };

	// 下方向の単位ベクトル
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	// ベクトル間角度の算出でゼロ扱いする分母の閾値
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	// 点が左上基準の矩形内にあるか判定
	static bool IsPointInRect(const Vector2& point, const Vector2& leftTop, const Vector2& size);

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
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

	// 三次元ベクトルの線形補間
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 度指定の角度の最短方向の線形補間（未使用）
	static double LerpDeg(double start, double end, double t);

	// 色成分の線形補間（未使用）
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// 整数二次元座標の二次ベジェ補間（未使用）
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);

	// 三次元座標の二次ベジェ補間（未使用）
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);

	// 中心座標の周囲でのY軸回転（未使用）
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const Vector2& v);

	// 三次元ベクトルの長さの取得（未使用）
	static double Magnitude(const VECTOR& v);

	// 三次元ベクトルの長さのfloat型取得
	static float MagnitudeF(const VECTOR& v);

	// 整数二次元ベクトルの長さの二乗の取得（未使用）
	static int SqrMagnitude(const Vector2& v);

	// 三次元ベクトルの長さの二乗のfloat型取得（未使用）
	static float SqrMagnitudeF(const VECTOR& v);

	// 三次元ベクトルの長さの二乗の取得
	static double SqrMagnitude(const VECTOR& v);

	// 三次元座標間の距離の二乗の取得（未使用）
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);

	// 整数二次元座標間の距離の取得（未使用）
	static double Distance(const Vector2& v1, const Vector2& v2);

	// 三次元座標間の距離の取得（未使用）
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定（未使用）
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius, 
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 三次元ベクトルの全成分の一致判定（未使用）
	static bool Equals(const VECTOR& v1, const VECTOR& v2);

	// ゼロベクトルとの一致判定
	static bool EqualsVZero(const VECTOR& v1);

	// 整数二次元ベクトルの正規化（未使用）
	static VECTOR Normalize(const Vector2& v);

	// 三次元ベクトルの正規化
	static VECTOR VNormalize(const VECTOR& v);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = DEFAULT_DEBUG_LINE_LENGTH);

	// 回転行列に基づくXYZ軸のデバッグ描画（未使用）
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = DEFAULT_DEBUG_LINE_LENGTH);

	// クォータニオンに基づくXYZ軸のデバッグ描画（未使用）
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = DEFAULT_DEBUG_LINE_LENGTH);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);
private:

	// デバッグ表示の球マーカーの半径
	static constexpr float DEBUG_MARKER_RADIUS = 5.0f;

	// デバッグ表示の球マーカーの分割数
	static constexpr int DEBUG_MARKER_DIVISIONS = 5;

	// X軸のデバッグ表示色
	static constexpr int DEBUG_AXIS_X_COLOR = 0xff0000;

	// Y軸のデバッグ表示色
	static constexpr int DEBUG_AXIS_Y_COLOR = 0x00ff00;

	// Z軸のデバッグ表示色
	static constexpr int DEBUG_AXIS_Z_COLOR = 0x0000ff;

};


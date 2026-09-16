#pragma once
#include <DxLib.h>
#include <algorithm>
class Quaternion
{

public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	// コンストラクタ
	Quaternion();
	explicit Quaternion(const VECTOR& rad);
	Quaternion(double w, double x, double y, double z);

	// デストラクタ
	~Quaternion();

	// オイラー角からクォータニオンへ変換
	static Quaternion Euler(const VECTOR& rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// 座標を回転させる
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);
	VECTOR PosAxis(VECTOR pos) const;

	// クォータニオンからオイラー角へ変換
	static VECTOR ToEuler(const Quaternion& q);
	VECTOR ToEuler() const;

	// クォータニオンから行列へ変換
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix() const;

	// ベクトルからクォータニオンに変換
	static Quaternion LookRotation(const VECTOR& dir);
	static Quaternion LookRotation(const VECTOR& dir, const VECTOR& up);

	// 行列からクォータニオンに変換
	static Quaternion GetRotation(const MATRIX& mat);

	// 基本ベクトルを取得
	VECTOR GetForward() const;
	VECTOR GetBack() const;
	VECTOR GetRight() const;
	VECTOR GetLeft() const;
	VECTOR GetUp() const;
	VECTOR GetDown() const;

	// 内積
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;

	// 正規化
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized() const;
	void Normalize();

	// 逆クォータニオン
	Quaternion Inverse() const;

	// 球面補間
	static Quaternion Slerp(const Quaternion& from, const Quaternion& to, double t);

	// ２つのベクトル間の回転量を取得する
	static Quaternion FromToRotation(const VECTOR& fromDir, const VECTOR& toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity();

	// 長さ
	double Length() const;
	double LengthSquared() const;
	VECTOR xyz() const;

	// 対象方向の回転
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	Quaternion operator*(float rhs);
	Quaternion operator+(const Quaternion& rhs);
	
};

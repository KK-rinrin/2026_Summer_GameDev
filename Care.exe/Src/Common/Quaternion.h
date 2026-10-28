#pragma once
#include <DxLib.h>
#include <algorithm>
class Quaternion
{

public:

	// 正規化判定用の微小値（未使用）
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	// クォータニオンの実数成分
	double w;

	// クォータニオンのX成分
	double x;

	// クォータニオンのY成分
	double y;

	// クォータニオンのZ成分
	double z;

	// コンストラクタ
	Quaternion();

	// ラジアン指定のオイラー角による初期化
	explicit Quaternion(const VECTOR& rad);

	// 実数成分とXYZ成分による初期化
	Quaternion(double w, double x, double y, double z);

	// デストラクタ
	~Quaternion();

	// オイラー角ベクトルからの回転の生成（未使用）
	static Quaternion Euler(const VECTOR& rad);

	// XYZ軸のラジアン指定による回転の生成
	static Quaternion Euler(double radX, double radY, double radZ);

	// クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);

	// 自身と指定クォータニオンの積
	Quaternion Mult(const Quaternion& q) const;

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// 座標を回転させる
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);

	// 自身の回転を適用した座標の取得
	VECTOR PosAxis(VECTOR pos) const;

	// クォータニオンからオイラー角へ変換
	static VECTOR ToEuler(const Quaternion& q);

	// 自身の回転に対応するオイラー角の取得（未使用）
	VECTOR ToEuler() const;

	// クォータニオンから行列へ変換
	static MATRIX ToMatrix(const Quaternion& q);

	// 自身の回転に対応する行列の取得（未使用）
	MATRIX ToMatrix() const;

	// 前方向のみを指定した回転の生成（未使用）
	static Quaternion LookRotation(const VECTOR& dir);

	// 前方向と上方向を指定した回転の生成
	static Quaternion LookRotation(const VECTOR& dir, const VECTOR& up);

	// 回転行列に対応するクォータニオンの取得（未使用）
	static Quaternion GetRotation(const MATRIX& mat);

	// 回転後の前方向ベクトルの取得
	VECTOR GetForward() const;

	// 回転後の後方向ベクトルの取得（未使用）
	VECTOR GetBack() const;

	// 回転後の右方向ベクトルの取得
	VECTOR GetRight() const;

	// 回転後の左方向ベクトルの取得（未使用）
	VECTOR GetLeft() const;

	// 回転後の上方向ベクトルの取得
	VECTOR GetUp() const;

	// 回転後の下方向ベクトルの取得（未使用）
	VECTOR GetDown() const;

	// 内積
	static double Dot(const Quaternion& q1, const Quaternion& q2);

	// 自身と指定クォータニオンの内積（未使用）
	double Dot(const Quaternion& b) const;

	// 正規化
	static Quaternion Normalize(const Quaternion& q);

	// 正規化済みクォータニオンの取得
	Quaternion Normalized() const;

	// 自身の各成分の正規化
	void Normalize();

	// 逆クォータニオン
	Quaternion Inverse() const;

	// 補間率を0から1に制限した球面線形補間（未使用）
	static Quaternion Slerp(const Quaternion& from, const Quaternion& to, double t);

	// 二つの方向ベクトル間の回転の取得（未使用）
	static Quaternion FromToRotation(const VECTOR& fromDir, const VECTOR& toDir);

	// 最大回転角度を制限した目標回転への補間（未使用）
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);

	// 二つのクォータニオン間の角度の取得
	static double Angle(const Quaternion& q1, const Quaternion& q2);

	// 補間率を制限しない球面線形補間
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);

	// 回転なしを表す単位クォータニオンの取得
	static Quaternion Identity();

	// 長さ
	double Length() const;

	// クォータニオンの長さの二乗の取得
	double LengthSquared() const;

	// XYZ成分のベクトルとしての取得
	VECTOR xyz() const;

	// 自身の回転角度と回転軸の取得（未使用）
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	// 逆向きに近いベクトルと判定する角度の閾値
	static constexpr double ANTIPARALLEL_ANGLE_DEGREES = 179.9196;

	// 回転軸の長さの二乗に対する微小値
	static constexpr float AXIS_LENGTH_SQUARED_EPSILON = 0.000001f;

	// 球面補間から線形補間へ切り替える余弦の閾値
	static constexpr float SLERP_LINEAR_COS_THRESHOLD = 0.99f;

	// 回転軸算出時の分母に対する微小値
	static constexpr float ANGLE_AXIS_DENOMINATOR_EPSILON = 0.0001f;


	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	// 各成分とスカラーの積（未使用）
	Quaternion operator*(float rhs);

	// 対応する成分同士の加算（未使用）
	Quaternion operator+(const Quaternion& rhs);
	
};

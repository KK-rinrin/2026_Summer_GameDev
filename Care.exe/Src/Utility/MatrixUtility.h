#pragma once
#include <DxLib.h>

class MatrixUtility
{
public:
	// オイラー角から回転行列XYZ順を取得
	static MATRIX GetMatrixRotateXYZ(const VECTOR& euler);

	// 親子の回転行列の合成（未使用）
	static MATRIX Multiplication(const MATRIX& child, const MATRIX& parent);

	// 親子のオイラー角から生成した回転行列の合成（未使用）
	static MATRIX Multiplication(const VECTOR& childEuler, const VECTOR& parentEuler);

};

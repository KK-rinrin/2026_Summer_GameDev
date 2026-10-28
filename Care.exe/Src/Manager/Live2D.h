#pragma once

#include <DxLib.h>
#include "ResourceManager.h"

class Live2D
{
public:
	Live2D();
	~Live2D();

	// リソースに対応するLive2Dモデルの読み込み
	void Load(ResourceManager::SRC src);
	void Update(const TCHAR* motion);
	// 即時に指定モーションを再生したいときに呼ぶ（controller から直接再生）
	void StartMotion(const TCHAR* motion, int priority = 0);

	// 共有描画回数に応じたLive2D描画の開始
	void DrawBegin();

	// Live2Dモデル本体の描画
	void DrawModel();

	// 共有描画回数に応じたLive2D描画の終了
	void DrawEnd();

	// Live2Dモデルハンドルの解放
	void DeleteModel();

	// Live2Dモデルの縦横倍率の設定
	void SetExtendRate(float x, float y);

	// Live2Dモデルの表示位置の設定
	void SetTranslate(float x, float y);

	// 指定IDのLive2Dパラメーター値の設定
	void SetParamerterValue(const TCHAR* parameterId, float value);

	// デバッグ／状態チェック用アクセサ
	int GetHandle() const { return modelHandle; }

private:

	// モデル更新に使用する固定時間の秒数
	static constexpr float UPDATE_DELTA_SECONDS = 1 / 60.0f;

	// 読み込み済みLive2Dモデルのハンドル
	int modelHandle;

	// RenderBegin/End の参照カウント（全インスタンスで共有）
	static int s_renderCount;
};

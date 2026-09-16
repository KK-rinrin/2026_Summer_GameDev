#pragma once

class Fader
{

public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};

	// コンストラクタ
	Fader();

	// デストラクタ
	~Fader();

	// 状態の取得
	STATE GetState() const;

	// フェード処理が終了しているか
	bool IsEnd() const;

	// 指定フェードを開始する
	void SetFade(STATE state);

	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

};

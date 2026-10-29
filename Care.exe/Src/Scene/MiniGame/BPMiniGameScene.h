#pragma once
#include "../../Application.h"
#include "../../Common/Vector2.h"
#include "../SceneBase.h"

class BPMiniGameScene : public SceneBase
{
public:
	BPMiniGameScene();
	~BPMiniGameScene() override;

	void Update() override;
	void Draw() override;
	void Delete() override;
	static constexpr Vector2<int> OK_RELATIVE_POS = { 178, 18 }; // 成功表示OKの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> PULSE_WAVE_RELATIVE_POS = { 154, 46 }; // 脈波の開始位置（表示領域の左上基準、px）

private:

	// 描画色のRGB成分の組
	struct Rgb
	{

		// 各色成分
		int r, g, b;

		// RGB成分からカラーコードへの変換
		unsigned int ToDxColor() const { return GetColor(r, g, b); }
	};

	// 数値表示と異常時の揺れ（フレームはUpdateの呼び出し回数）
	static constexpr int DISPLAY_VALUE_MAX = 199; // 圧力と脈拍の表示上限（異常演出の加算後にも適用）
	static constexpr int PRESSURE_JITTER_FRAMES = 10; // 異常時に圧力表示の揺れを切り替える間隔（フレーム）
	static constexpr int PRESSURE_JITTER_AMPLITUDE = 4; // 異常時の圧力表示の増減幅（mmHg、判定には影響しない）
	static constexpr int STOPPED_PULSE_BLINK_FRAMES = 20; // 脈動停止中に偽の脈拍表示を切り替える間隔（フレーム）
	static constexpr int STOPPED_PULSE_VALUE = 69; // 異常時に脈動停止中でも表示する偽の脈拍（bpm）
	static constexpr int PULSE_BASE_VALUE = 72; // 脈動中の脈拍表示の基準値（bpm）
	static constexpr float PULSE_PHASE_STEP = 0.18f; // 脈拍数の揺れの速さ（rad/フレーム）
	static constexpr float PULSE_AMPLITUDE = 4.0f; // 脈拍数の通常の増減幅（bpm）
	static constexpr int ABNORMAL_PULSE_JITTER_FRAMES = 9; // 異常時に脈拍数の追加の揺れを切り替える間隔（フレーム）
	static constexpr int ABNORMAL_PULSE_JITTER_AMPLITUDE = 6; // 異常時に脈拍数へ追加する増減幅（bpm）
	static constexpr int TEXT_JITTER_FRAMES = 8; // 異常時に圧力と脈拍の文字を横揺れさせる間隔（フレーム）
	static constexpr int TEXT_JITTER_PIXELS = 2; // 文字の横揺れ幅（px、圧力と脈拍は逆向きに移動）
	static constexpr int INVALID_PULSE_BLINK_FRAMES = 10; // 異常時に脈拍表示を「???」へ切り替える間隔（フレーム）

	// 枠線
	static constexpr int MONITOR_FRAME_INSET = 10; // 代替描画のモニター外周から枠線までの距離（px）
	static constexpr int GAUGE_BORDER_WIDTH = 2; // 圧力ゲージの外側に枠線を置く距離（px、線の太さではない）

	// 腕の脈拍マーカー（画像がない場合は円で代替）
	static constexpr float MARKER_PHASE_STEP = 0.42f; // 脈拍マーカーの上下運動の速さ（rad/フレーム）
	static constexpr float MARKER_ABNORMAL_AMPLITUDE = 14.0f; // 異常時のマーカーの上下移動幅（片側、px）
	static constexpr float MARKER_NORMAL_AMPLITUDE = 10.0f; // 通常時のマーカーの上下移動幅（片側、px）
	static constexpr int MARKER_ACTIVE_RADIUS = 9; // 脈動中の代替マーカー円の基本半径（px）
	static constexpr float MARKER_RADIUS_AMPLITUDE = 5.0f; // 脈動に合わせて代替マーカー円へ加える半径（最大、px）
	static constexpr int MARKER_IDLE_RADIUS = 7; // 脈動停止中の代替マーカー円の半径（px）

	// モニター内の脈波
	static constexpr int WAVE_SAMPLE_COUNT = 92; // 脈波の線分数（1本につき横1pxなので描画幅も兼ねる）
	static constexpr float WAVE_PHASE_STEP = 0.25f; // 脈波の位相増分（rad、1フレームまたは横1pxごと）
	static constexpr float WAVE_ABNORMAL_AMPLITUDE = 20.0f; // 異常時の脈波の振幅（px、突出部分の増幅前）
	static constexpr float WAVE_NORMAL_AMPLITUDE = 14.0f; // 通常時の脈波の振幅（px、突出部分の増幅前）
	static constexpr int WAVE_SPIKE_PERIOD_FRAMES = 24; // 脈波の突出周期（フレーム＋横方向のサンプル番号に適用）
	static constexpr int WAVE_SPIKE_WIDTH_FRAMES = 4; // 周期内で増幅する幅（時間方向はフレーム、横方向はpx）
	static constexpr float WAVE_SPIKE_GAIN = 2.0f; // 脈波の突出部分に掛ける振幅の倍率
	static constexpr float WAVE_IDLE_PHASE_STEP = 0.12f; // 異常時・脈動停止中の微小波の位相増分（rad/フレームまたはpx）
	static constexpr float WAVE_IDLE_AMPLITUDE = 2.0f; // 異常時・脈動停止中の微小波の振幅（px）
	static constexpr int WAVE_LINE_WIDTH = 2; // 脈波の線の太さ（px）

	// モニター異常時の横ノイズとエラー表示
	static constexpr int GLITCH_LINE_COUNT = 4; // 異常時に重ねる横ノイズの本数
	static constexpr int GLITCH_X_INSET = 8; // 横ノイズの左端の余白と右端の切り詰め余白（px）
	static constexpr int GLITCH_X_STRIDE = 37; // ノイズ番号ごとの横位置のずらし量（折り返し前、px）
	static constexpr int GLITCH_X_RANGE = 40; // 横ノイズの開始X座標を循環させる範囲（px）
	static constexpr int GLITCH_Y_INSET = 14; // 横ノイズの開始Y座標の上余白（px）
	static constexpr int GLITCH_SPEED_BIAS = 3; // 先頭ノイズの縦移動速度（px/フレーム、番号ごとに1ずつ加算）
	static constexpr int GLITCH_Y_STRIDE = 29; // ノイズ番号ごとの初期Y座標のずらし量（折り返し前、px）
	static constexpr int GLITCH_Y_MARGIN = 28; // 縦位置の循環範囲を表示領域の高さから縮める量（px）
	static constexpr int GLITCH_MIN_LENGTH = 60; // 横ノイズの最短長さ（右端での切り詰め前、px）
	static constexpr int GLITCH_LENGTH_STRIDE = 23; // ノイズ番号ごとの長さの増分（折り返し前、px）
	static constexpr int GLITCH_LENGTH_OFFSET = 34; // 先頭ノイズの長さに加える値（折り返し前、px）
	static constexpr int GLITCH_LENGTH_RANGE = 140; // ノイズの追加長さを循環させる範囲（px）
	static constexpr int GLITCH_LINE_HEIGHT = 3; // 横ノイズの高さ（px）
	static constexpr int ERROR_BLINK_FRAMES = 24; // ERRの表示・非表示を切り替える間隔（フレーム）

	// 描画色（RGB、各成分0～255）
	static constexpr Rgb ROOM_COLOR = { 225, 229, 232 }; // 部屋の下地色
	static constexpr Rgb FLOOR_COLOR = { 204, 214, 218 }; // 床の下地色
	static constexpr Rgb BED_COLOR = { 238, 242, 242 }; // 背景画像がない場合のベッドの色
	static constexpr Rgb BED_SHADOW_COLOR = { 178, 189, 194 }; // 背景画像がない場合のベッドの影色
	static constexpr Rgb LIMB_COLOR = { 235, 186, 156 }; // 背景画像がない場合の腕の色
	static constexpr Rgb WRAP_COLOR = { 38, 55, 71 }; // 背景画像がない場合の腕に巻くカフの色
	static constexpr Rgb MONITOR_COLOR = { 34, 38, 42 }; // モニター画像がない場合の本体色
	static constexpr Rgb MONITOR_FRAME_COLOR = { 72, 78, 84 }; // モニター画像がない場合の外枠の色
	static constexpr Rgb DISPLAY_COLOR = { 8, 28, 26 }; // モニター画像がない場合の表示領域の下地色
	static constexpr Rgb STEM_COLOR = { 46, 52, 56 }; // モニター画像がない場合の支柱の色
	static constexpr Rgb BASE_COLOR = { 44, 49, 53 }; // モニター画像がない場合の台座の色
	static constexpr Rgb TEXT_COLOR = { 170, 245, 214 }; // 圧力・脈拍・測定結果の通常の文字色
	static constexpr Rgb SUB_TEXT_COLOR = { 112, 178, 154 }; // 項目名・単位・圧力ゲージの枠線の色
	static constexpr Rgb ABNORMAL_TEXT_COLOR = { 255, 120, 120 }; // 異常な脈拍表示「???」の文字色
	static constexpr Rgb SUCCESS_COLOR = { 120, 235, 130 }; // 成功表示OKの文字色
	static constexpr Rgb GAUGE_BG_COLOR = { 18, 58, 52 }; // 圧力ゲージの空き部分の色
	static constexpr Rgb GAUGE_FILL_COLOR = { 112, 228, 183 }; // 圧力ゲージの充填部分の色
	static constexpr Rgb PULSE_ACTIVE_COLOR = { 235, 62, 74 }; // 脈動中の代替マーカー円の色
	static constexpr Rgb PULSE_IDLE_COLOR = { 116, 101, 104 }; // 脈動停止中の代替マーカー円の色
	static constexpr Rgb WAVE_COLOR = { 96, 230, 170 }; // 脈波の線の色
	static constexpr Rgb GLITCH_EVEN_COLOR = { 42, 116, 114 }; // 偶数番号の横ノイズの色（0番から数える）
	static constexpr Rgb GLITCH_ODD_COLOR = { 112, 42, 70 }; // 奇数番号の横ノイズの色
	static constexpr Rgb ERROR_COLOR = { 235, 76, 90 }; // 異常表示ERRの文字色
	static constexpr Rgb FAILED_COLOR = { 225, 60, 70 }; // 失敗画像がない場合のFAILEDの文字色
	static constexpr Rgb BLACK_COLOR = { 0, 0, 0 }; // 失敗画面の黒い下地色

	// 血圧測定の進行状態
	enum class State
	{
		INFLATE,	// 加圧中
		DEFLATE,	// 減圧と血圧検出中
		SUCCESS,	// 血圧測定の成功
		FAILED,		// 血圧測定の失敗
	};

	// ミニゲーム画像の配列インデックス
	enum class Image
	{
		BACKGROUND,	// 背景画像
		MONITOR,	// 血圧計モニター画像
		PULSE,		// 脈拍マーカー画像
		FAILED,		// 測定失敗画像
		INTRUCT,	// 操作説明画像
		MAX			// 画像総数
	};


	// 文字のサイズと太さ
	static constexpr int FONT_SIZE = 24; // 項目名・単位・測定結果に使う文字サイズ
	static constexpr int LARGE_FONT_SIZE = 42; // 圧力・脈拍・OK・FAILEDに使う文字サイズ
	static constexpr int FONT_THICK = 3; // 通常フォントの太さ（LoadFontへ渡す値）
	static constexpr int LARGE_FONT_THICK = 5; // 大きいフォントの太さ（LoadFontへ渡す値）

	// 測定ルール・圧力の変化速度
	static constexpr float PRESSURE_MIN = 0.0f; // 圧力の下限（mmHg、開始時の圧力も兼ねる）
	static constexpr float PRESSURE_MAX = 180.0f; // 加圧を終えて減圧へ移る圧力（mmHg、ゲージの最大値も兼ねる）
	static constexpr float INFLATE_SPEED = 72.0f; // 加圧操作中の圧力上昇速度（mmHg/秒）
	static constexpr float DEFLATE_SPEED = 5.0f; // 減圧中の圧力低下速度（mmHg/秒）
	static constexpr float SYSTOLIC_WINDOW_HIGH = 124.0f; // 最高血圧の入力受付を始める圧力（mmHg、脈動開始の境界）
	static constexpr float SYSTOLIC_WINDOW_LOW = 116.0f; // 最高血圧の入力受付の下限（mmHg、未測定で下回ると失敗）
	static constexpr float DIASTOLIC_WINDOW_HIGH = 82.0f; // 最低血圧の入力受付を始める圧力（mmHg、脈動停止の境界）
	static constexpr float DIASTOLIC_WINDOW_LOW = 74.0f; // 最低血圧の入力受付の下限（mmHg、最高血圧測定後に下回ると失敗）
	static constexpr float SUCCESS_WAIT_SEC = 0.6f; // 測定成功からゲーム画面へ戻るまでの待ち時間（秒）
	static constexpr int MONITOR_ABNORMAL_PROGRESS = 8; // モニターの異常演出を有効にするゲーム進行値の下限

	// 画面配置（位置とサイズはpx）
	static constexpr Vector2<int> SCREEN_SIZE = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y }; // 画面全体の幅・高さ（px）
	static constexpr Vector2<int> MONITOR_POS = { 240, 64 }; // モニター本体の左上位置（画面基準、px）
	static constexpr Vector2<int> MONITOR_SIZE = { 320, 250 }; // モニター本体の幅・高さ（px、画像の描画中心計算にも使用）
	static constexpr Vector2<int> DISPLAY_POS = { MONITOR_POS.x + 28, MONITOR_POS.y + 30 }; // 表示領域の左上位置（画面基準、px）
	static constexpr Vector2<int> DISPLAY_SIZE = { MONITOR_SIZE.x - 56, 178 }; // 表示領域の幅・高さ（px）
	static constexpr Vector2<int> PULSE_MARKER_POS = { 394, 314 }; // 脈拍マーカーの中心位置（画面基準、上下運動前、px）
	static constexpr Vector2<int> FLOOR_POS = { 0, 418 }; // 床の左上位置（画面基準、px）
	static constexpr Vector2<int> FLOOR_SIZE = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y - FLOOR_POS.y }; // 床の幅・高さ（px）
	static constexpr Vector2<int> BED_POS = { 86, 350 }; // 代替描画のベッドの左上位置（画面基準、px）
	static constexpr Vector2<int> BED_SIZE = { 358, 138 }; // 代替描画のベッドの幅・高さ（px）
	static constexpr Vector2<int> BED_SHADOW_POS = { 86, 480 }; // 代替描画のベッドの影の左上位置（画面基準、px）
	static constexpr Vector2<int> BED_SHADOW_SIZE = { 358, 20 }; // 代替描画のベッドの影の幅・高さ（px）
	static constexpr Vector2<int> FALLBACK_LIMB_CENTER = { 250, 380 }; // 代替描画の腕の楕円の中心（画面基準、px）
	static constexpr Vector2<int> FALLBACK_LIMB_RADIUS = { 180, 48 }; // 代替描画の腕の楕円の横半径・縦半径（px）
	static constexpr Vector2<int> FALLBACK_WRAP_POS = { 244, 316 }; // 代替描画のカフの左上位置（画面基準、px）
	static constexpr Vector2<int> FALLBACK_WRAP_SIZE = { 118, 118 }; // 代替描画のカフの幅・高さ（px）

	// モニター画像がない場合の支柱・台座
	static constexpr Vector2<int> MONITOR_STEM_POS = { 130, MONITOR_SIZE.y }; // 代替描画の支柱の左上位置（モニター左上基準、px）
	static constexpr Vector2<int> MONITOR_STEM_SIZE = { 60, 50 }; // 代替描画の支柱の幅・高さ（px）
	static constexpr Vector2<int> MONITOR_BASE_POS = { 82, MONITOR_SIZE.y + 48 }; // 代替描画の台座の左上位置（モニター左上基準、px）
	static constexpr Vector2<int> MONITOR_BASE_SIZE = { 156, 20 }; // 代替描画の台座の幅・高さ（px）

	// 表示領域内の文字と圧力ゲージの配置
	static constexpr Vector2<int> PRESSURE_LABEL_OFFSET = { 18, 14 }; // PRESSUREの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> PRESSURE_VALUE_OFFSET = { 24, 42 }; // 圧力数値の左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> PRESSURE_UNIT_OFFSET = { 158, 68 }; // 単位mmHgの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> PULSE_LABEL_OFFSET = { 18, 98 }; // PULSEの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> PULSE_VALUE_OFFSET = { 28, 120 }; // 脈拍数値の左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> SYSTOLIC_LABEL_OFFSET = { 142, 112 }; // 最高血圧の項目名SYSの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> SYSTOLIC_VALUE_OFFSET = { 196, 112 }; // 最高血圧の測定結果の左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> DIASTOLIC_LABEL_OFFSET = { 142, 142 }; // 最低血圧の項目名DIAの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> DIASTOLIC_VALUE_OFFSET = { 196, 142 }; // 最低血圧の測定結果の左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> PRESSURE_GAUGE_POS = { MONITOR_POS.x + MONITOR_SIZE.x - 42, DISPLAY_POS.y + 18 }; // 圧力ゲージの左上位置（画面基準、px）
	static constexpr Vector2<int> PRESSURE_GAUGE_SIZE = { 10, DISPLAY_SIZE.y - 36 }; // 圧力ゲージの幅・高さ（px）
	static constexpr Vector2<int> ERROR_TEXT_OFFSET = { 154, 20 }; // ERRの左上位置（表示領域の左上基準、px）
	static constexpr Vector2<int> FAILED_TEXT_CENTER_OFFSET = { 108, 34 }; // FAILEDの左上を画面中心から左・上にずらす量（px）


	void InitLoad() override;

	// ミニゲームに使用する画像の読み込み
	void LoadImages();

	// 入力に応じた加圧状態の更新
	void UpdateInflate(float deltaTime);

	// 減圧と血圧検出入力の更新
	void UpdateDeflate(float deltaTime);

	// 測定成功後の復帰待ち時間の更新
	void UpdateSuccess(float deltaTime);

	// 測定失敗後の入力状態の更新
	void UpdateFailed();

	// 測定失敗状態への切り替え
	void Fail();

	// 現在圧力の最高血圧としての記録
	void RecordSystolic();

	// 現在圧力の最低血圧としての記録
	void RecordDiastolic();

	// 最高血圧を記録可能な圧力範囲の判定
	bool IsSystolicDetectable() const;

	// 最低血圧を記録可能な圧力範囲の判定
	bool IsDiastolicDetectable() const;

	// 現在圧力での脈動状態の判定
	bool IsPulseMoving() const;

	// 最高血圧の検出機会の通過判定
	bool IsPulseMissed() const;

	// 最低血圧の検出機会の通過判定
	bool IsDiastolicMissed() const;

	// 異常演出と上限を反映した表示圧力の取得
	int GetDisplayPressure() const;

	// 異常演出と脈動状態を反映した表示脈拍の取得
	int GetDisplayPulse() const;

	// 背景素材がない場合の部屋の代替描画
	void DrawRoom() const;

	// 背景画像の描画
	void DrawBG() const;

	// モニターと測定数値の描画
	void DrawMonitor() const;

	// 圧力ゲージの描画
	void DrawPressureGauge() const;

	// 脈動に合わせた腕のマーカーの描画
	void DrawPulseMarker() const;

	// モニター内の脈波の描画
	void DrawPulseWave() const;

	// モニターの横ノイズとエラー表示の描画
	void DrawGlitch() const;

	// 測定失敗画面の描画
	void DrawFailed() const;

	// 整数値の指定範囲への制限
	static int ClampInt(int value, int minValue, int maxValue);

	// 二つの整数の最大値の取得（未使用）
	static int MaxInt(int lhs, int rhs);

	// 二つの整数の最小値の取得
	static int MinInt(int lhs, int rhs);

	// 現在の血圧測定の進行状態
	State state_;

	// 現在のカフ圧力
	float pressure_;

	// 成功結果の表示経過秒数
	float resultTimer_;

	// 演出用の経過フレーム数
	int frame_;

	// 通常文字用フォントのハンドル
	int font_;

	// 数値表示用の大きいフォントのハンドル
	int largeFont_;

	// 画像種別ごとの読み込み済みハンドル
	int images_[static_cast<int>(Image::MAX)];

	// 記録済みの最高血圧
	int systolic_;

	// 記録済みの最低血圧
	int diastolic_;

	// モニターの異常演出の有効状態
	bool isMonitorAbnormal_;

	// 最高血圧の測定済み状態
	bool isSystolicMeasured_;

	// リソースの解放済み状態
	bool isDeleted_;

};

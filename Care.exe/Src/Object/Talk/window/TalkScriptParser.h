#pragma once
#include <string>
#include <vector>

// ==============================
// 文字ユニット定義
// ==============================
struct CharUnit
{

	// 文字ユニットの処理種別
	enum class Type
	{

		// 通常の表示文字
		NORMAL,

		// 待機を伴う句読点文字
		PUNCT,

		// 文字表示を伴わない制御タグ
		TAG
	};

	// 句読点種別
	enum class PunctKind
	{

		// 句読点の指定なし
		NONE,
		PERIOD,    // 。 .
		COMMA,     // 、 ,
		EXCLAM,    // !
		QUESTION,  // ?
		EXCLAM_MANY,   // !!! 等
		QUESTION_MANY, // ??? 等
		MIXED          // !? ?! 等
	};

	// 文字ユニットの処理種別
	Type type = Type::NORMAL;

	// 表示文字
	std::string text;

	// タグ
	std::string tagType;   // タグ名 ("WAIT", "FBF", "PARAM", "HAPPY" 等)

	// 制御タグの追加パラメーター
	std::string tagParam;  // タグの追加パラメータ（PARAM の名前等）

	// 制御タグによる待機時間のミリ秒数
	int   waitMs = 0;

	// 制御タグが指定する数値
	float floatValue = 1.0f;

	// 句読点
	PunctKind punctKind = PunctKind::NONE;

	// フラグ：このユニットは「括弧内」であり口パクを行わない
	bool noMouth = false;
};

// ==============================
// スクリプトパーサ
// ==============================
class TalkScriptParser
{
public:
	// 文字列を CharUnit に分解
	static std::vector<CharUnit> Parse(const std::string& src);

private:

	// Shift-JISの前半リードバイト領域の最小値
	static constexpr unsigned char SJIS_LEAD_LOW_MIN = 0x81;

	// Shift-JISの前半リードバイト領域の最大値
	static constexpr unsigned char SJIS_LEAD_LOW_MAX = 0x9F;

	// Shift-JISの後半リードバイト領域の最小値
	static constexpr unsigned char SJIS_LEAD_HIGH_MIN = 0xE0;

	// Shift-JISの後半リードバイト領域の最大値
	static constexpr unsigned char SJIS_LEAD_HIGH_MAX = 0xFC;

	// Shift-JISの全角文字のバイト数
	static constexpr std::size_t SJIS_CHARACTER_BYTES = 2;

	// Shift-JIS 1文字読み
	static bool ReadCharSJIS(const std::string& src, size_t& i, std::string& outChar);

	// 句読点判定
	static CharUnit::PunctKind IsPunctuation(const std::string& ch);
};

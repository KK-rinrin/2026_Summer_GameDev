#pragma once
#include <string>
#include <vector>

// ==============================
// 文字ユニット定義
// ==============================
struct CharUnit
{
    enum class Type
    {
        NORMAL,
        PUNCT,
        TAG
    };

    // 句読点種別
    enum class PunctKind
    {
        NONE,
        PERIOD,    // 。 .
        COMMA,     // 、 ,
        EXCLAM,    // !
        QUESTION,  // ?
        EXCLAM_MANY,   // !!! 等
        QUESTION_MANY, // ??? 等
        MIXED          // !? ?! 等
    };

    Type type = Type::NORMAL;

    // 表示文字
    std::string text;

    // TAG
    std::string tagType;   // タグ名 ("WAIT", "FBF", "PARAM", "HAPPY" 等)
    std::string tagParam;  // タグの追加パラメータ（PARAM の名前等）
    int   waitMs = 0;
    float floatValue = 1.0f;

    // PUNCT
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
    // Shift-JIS 1文字読み
    static bool ReadCharSJIS(const std::string& src, size_t& i, std::string& outChar);

    // 句読点判定
    static CharUnit::PunctKind IsPunctuation(const std::string& ch);
};
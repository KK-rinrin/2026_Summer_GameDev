#include "TalkScriptParser.h"
#include <cstdlib>

// ==============================
// パース実装
// ==============================
std::vector<CharUnit> TalkScriptParser::Parse(const std::string& src)
{
    std::vector<CharUnit> units;
    units.reserve(src.size());

    size_t i = 0;
    int parenDepth = 0; // 丸括弧の深さ（全角/半角両対応）
    while (i < src.size())
    {
        // -------- ブレースタグ --------
        if (src[i] == '{')
        {
            size_t end = src.find('}', i);
            if (end != std::string::npos)
            {
                std::string inside = src.substr(i + 1, end - i - 1);

                CharUnit cu;
                cu.type = CharUnit::Type::TAG;
                cu.noMouth = (parenDepth > 0);

                size_t colon = inside.find(':');
                if (colon != std::string::npos)
                {
                    cu.tagType = inside.substr(0, colon);
                    std::string param = inside.substr(colon + 1);

                    if (cu.tagType == "WAIT")
                        cu.waitMs = std::atoi(param.c_str());
                    else if (cu.tagType == "FBF")
                        cu.floatValue = static_cast<float>(std::atof(param.c_str()));
                    else if (cu.tagType == "PARAM")
                    {
                        // PARAM:名前:値 か PARAM:名前 の想定
                        size_t colon2 = param.find(':');
                        if (colon2 != std::string::npos)
                        {
                            cu.tagParam = param.substr(0, colon2);
                            cu.floatValue = static_cast<float>(std::atof(param.substr(colon2 + 1).c_str()));
                        }
                        else
                        {
                            cu.tagParam = param;
                            // 値省略なら既定値 1.0f のまま
                        }
                    }
                    else
                    {
                        // その他のタグはパラメータ部を tagParam に保持（例: {EXPR:0.8} など）
                        cu.tagParam = param;
                        // もし param が数値であれば floatValue に設定しておく
                        cu.floatValue = static_cast<float>(std::atof(param.c_str()));
                    }
                }
                else
                {
                    cu.tagType = inside;
                }

                units.push_back(cu);
                i = end + 1;
                continue;
            }
        }

        // -------- 1文字読み（Shift-JIS対応） --------
        std::string ch;
        if (!ReadCharSJIS(src, i, ch))
            break;

        // 丸括弧の判定（全角/半角）
        bool isOpen = (ch == "(" || ch == "（");
        bool isClose = (ch == ")" || ch == "）");

        // -------- 句読点判定 --------
        if (IsPunctuation(ch) != CharUnit::PunctKind::NONE)
        {
            CharUnit cu;
            cu.type = CharUnit::Type::PUNCT;
            cu.text = ch;

            // 括弧開始はその括弧から noMouth に含めたい
            if (isOpen)
            {
                parenDepth++;
                cu.noMouth = true;
            }
            else
            {
                cu.noMouth = (parenDepth > 0);
                if (isClose && parenDepth > 0)
                    parenDepth--;
            }

            units.push_back(cu);
            continue;
        }

        // -------- 通常文字 --------
        CharUnit cu;
        cu.type = CharUnit::Type::NORMAL;
        cu.text = ch;

        if (isOpen)
        {
            parenDepth++;
            cu.noMouth = true;
        }
        else if (isClose)
        {
            cu.noMouth = (parenDepth > 0);
            if (parenDepth > 0) parenDepth--;
        }
        else
        {
            cu.noMouth = (parenDepth > 0);
        }

        units.push_back(cu);
    }

    return units;
}

// ==============================
// Shift-JIS 1文字読み
// ==============================
bool TalkScriptParser::ReadCharSJIS(
    const std::string& src, size_t& i, std::string& outChar)
{
    if (i >= src.size()) return false;

    unsigned char c = static_cast<unsigned char>(src[i]);
    outChar.clear();

    // 2バイト文字判定
    if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC))
    {
        if (i + 1 < src.size())
        {
            outChar += src[i];
            outChar += src[i + 1];
            i += 2;
        }
        else
        {
            outChar += src[i];
            i++;
        }
    }
    else
    {
        // 1バイト文字
        outChar += src[i];
        i++;
    }

    return true;
}

// ==============================
// 句読点判定
// ==============================
CharUnit::PunctKind TalkScriptParser::IsPunctuation(const std::string& ch)
{
    if (ch == "。" || ch == "." || ch == "．")
        return CharUnit::PunctKind::PERIOD;

    if (ch == "、" || ch == "," || ch == "，" || ch == "、")
        return CharUnit::PunctKind::COMMA;

    if (ch == "！" || ch == "!")
        return CharUnit::PunctKind::EXCLAM;

    if (ch == "？" || ch == "?")
        return CharUnit::PunctKind::QUESTION;

    // 複合記号判定
    if (ch.find("!") != std::string::npos &&
        ch.find("?") != std::string::npos)
        return CharUnit::PunctKind::MIXED;

    if (ch.find("!") != std::string::npos)
        return CharUnit::PunctKind::EXCLAM_MANY;

    if (ch.find("?") != std::string::npos)
        return CharUnit::PunctKind::QUESTION_MANY;

    return CharUnit::PunctKind::NONE;
}
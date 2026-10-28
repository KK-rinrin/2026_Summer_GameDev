#pragma once
#include <memory>
#include <DxLib.h>
#include <string>
#include <vector>

class Resource
{
public:
	// リソースタイプ
	enum class TYPE
	{

		// 未指定のリソース種別
		NONE,

		// 単一画像のリソース
		IMG,

		// 分割画像のリソース
		IMGS,

		// 三次元モデルのリソース
		MODEL,

		// Live2Dモデルのリソース
		LIVE2D,

		// フォントのリソース
		FONT,

		// 音声のリソース
		SOUND,
	};

	// コンストラクタ
	Resource();
	// コンストラクタ
	Resource(TYPE type, const std::string& path);
	// コンストラクタ(IMGS/FONT用)
	// IMGSの場合 value1 = numX, value2 = numY, value3 = sizeX, value4 = sizeY
	// FONTの場合 value1 = fontSize, value2 = thick, value3 = fontSpace, value4 = fontType
	Resource(TYPE type, const std::string& path, int value1, int value2 = -1, int value3 = 0, int value4 = DX_FONTTYPE_ANTIALIASING);
	

	// デストラクタ
	~Resource();

	// 読み込み
	void Load();

	// フォントを指定設定で読み込み
	int LoadFont(int fontSize, int thick = -1, int fontSpace = 0, int fontType = DX_FONTTYPE_ANTIALIASING);

	// 解放
	void Release();

	// 分割画像ハンドルの外部配列へのコピー（未使用）
	void CopyHandle(int* imgs) const;

	// リソースタイプ
	TYPE type_;

	// リソースの読み込み先
	std::string path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	std::unique_ptr<int[]> handleIds_;

	// 分割画像の横方向のコマ数
	int numX_;

	// 分割画像の縦方向のコマ数
	int numY_;

	// 分割画像1コマの幅
	int sizeX_;

	// 分割画像1コマの高さ
	int sizeY_;

	// 既定フォントの文字サイズ
	int fontSize_;

	// フォントの文字の太さ
	int thick_;

	// フォントの文字間隔
	int fontSpace_;

	// フォントの描画方式
	int fontType_;

	// モデル複製用
	std::vector<int> duplicateModelIds_;

private:

	// フォントの生成設定
	struct FontSetting
	{

		// フォントサイズ
		int size;

		// 文字の太さ
		int thick;

		// 文字間隔
		int space;

		// 文字の描画方式
		int type;
	};

	// 設定別の生成済みフォント情報
	struct FontVariant
	{

		// フォントの生成に使用した設定
		FontSetting setting;

		// 生成済みフォントのハンドル
		int handle;
	};

	// 設定別に保持するフォントの一覧
	std::vector<FontVariant> fontVariants_;

};

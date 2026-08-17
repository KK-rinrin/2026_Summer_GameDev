#pragma once
#include <DxLib.h>
#include <string>
#include <vector>

class Resource
{

public:
	
	// リソースタイプ
	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		MODEL,
		EFFEKSEER,
		LIVE2D,
		FONT,
		SOUND,
	};

	// コンストラクタ
	Resource(void);
	// コンストラクタ
	Resource(TYPE type, const std::string& path);
	// コンストラクタ(IMGS/FONT用)
	// IMGSの場合 value1 = numX, value2 = numY, value3 = sizeX, value4 = sizeY
	// FONTの場合 value1 = fontSize, value2 = thick, value3 = fontSpace, value4 = fontType
	Resource(TYPE type, const std::string& path, int value1, int value2 = -1, int value3 = 0, int value4 = DX_FONTTYPE_ANTIALIASING);
	

	// デストラクタ
	~Resource(void);

	// 読み込み
	void Load(void);

	// フォントを指定設定で読み込み
	int LoadFont(int fontSize, int thick = -1, int fontSpace = 0, int fontType = DX_FONTTYPE_ANTIALIASING);

	// 解放
	void Release(void);

	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs) const;

	// リソースタイプ
	TYPE type_;

	// リソースの読み込み先
	std::string path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	// FONT::CreateFontToHandle用
	int fontSize_;
	int thick_;
	int fontSpace_;
	int fontType_;

	// モデル複製用
	std::vector<int> duplicateModelIds_;

private:
	struct FontSetting
	{
		int size;
		int thick;
		int space;
		int type;
	};

	struct FontVariant
	{
		FontSetting setting;
		int handle;
	};

	std::vector<FontVariant> fontVariants_;

};

#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

Resource::Resource(void)
	:
	type_(TYPE::NONE),
	path_(""),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr),
	fontSize_(-1),
	thick_(-1),
	fontSpace_(0),
	fontType_(DX_FONTTYPE_ANTIALIASING)
{
}

Resource::Resource(TYPE type, const std::string& path)
	:
	type_(type),
	path_(path),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr),
	fontSize_(-1),
	thick_(-1),
	fontSpace_(0),
	fontType_(DX_FONTTYPE_ANTIALIASING)
{
}

Resource::Resource(TYPE type, const std::string& path, int value1, int value2, int value3, int value4)
	:
	type_(type),
	path_(path),
	numX_(type == TYPE::IMGS ? value1 : -1),
	numY_(type == TYPE::IMGS ? value2 : -1),
	sizeX_(type == TYPE::IMGS ? value3 : -1),
	sizeY_(type == TYPE::IMGS ? value4 : -1),
	handleId_(-1),
	handleIds_(nullptr),
	fontSize_(type == TYPE::FONT ? value1 : -1),
	thick_(type == TYPE::FONT ? value2 : -1),
	fontSpace_(type == TYPE::FONT ? value3 : 0),
	fontType_(type == TYPE::FONT ? value4 : DX_FONTTYPE_ANTIALIASING)
{
}

Resource::~Resource(void)
{
	Release();
}

void Resource::Load(void)
{

	switch (type_)
	{
	case Resource::TYPE::IMG:
		// 画像
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::IMGS:
		// 複数画像
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::TYPE::MODEL:
		// モデル
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:

		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;

	case Resource::TYPE::LIVE2D:

		handleId_ = Live2D_LoadModel(path_.c_str());
		break;

	case Resource::TYPE::FONT:
		handleId_ = LoadFont(fontSize_, thick_, fontSpace_, fontType_);
		break;

	case Resource::TYPE::SOUND:
		handleId_ = LoadSoundMem(path_.c_str());
		break;
	}

}

int Resource::LoadFont(int fontSize, int thick, int fontSpace, int fontType)
{
	if (type_ != TYPE::FONT)
	{
		return -1;
	}

	for (const auto& variant : fontVariants_)
	{
		if (variant.setting.size == fontSize
			&& variant.setting.thick == thick
			&& variant.setting.space == fontSpace
			&& variant.setting.type == fontType)
		{
			handleId_ = variant.handle;
			return handleId_;
		}
	}

	const int handle = CreateFontToHandle(path_.c_str(), fontSize, thick, fontType);
	if (handle == -1)
	{
		return -1;
	}

	SetFontSpaceToHandle(fontSpace, handle);

	FontVariant variant;
	variant.setting = { fontSize, thick, fontSpace, fontType };
	variant.handle = handle;
	fontVariants_.emplace_back(variant);
	handleId_ = handle;
	return handleId_;
}

void Resource::Release(void)
{

	switch (type_)
	{
	case Resource::TYPE::IMG:
		if (handleId_ != -1)
		{
			DeleteGraph(handleId_);
			handleId_ = -1;
		}
		break;

	case Resource::TYPE::IMGS:
	{
		if (handleIds_ != nullptr)
		{
			int num = numX_ * numY_;
			for (int i = 0; i < num; i++)
			{
				// handle が有効かどうか簡易チェック（0 は有効なハンドルの可能性があるので -1 を基準）
				if (handleIds_[i] != -1)
				{
					DeleteGraph(handleIds_[i]);
				}
			}
			delete[] handleIds_;
			handleIds_ = nullptr;
		}
	}
	break;

	case Resource::TYPE::MODEL:
	{
		if (handleId_ != -1)
		{
			MV1DeleteModel(handleId_);
			handleId_ = -1;
		}
		for (auto id : duplicateModelIds_)
		{
			if (id != -1)
			{
				MV1DeleteModel(id);
			}
		}
		duplicateModelIds_.clear();
	}
	break;

	case Resource::TYPE::EFFEKSEER:
		if (handleId_ != -1)
		{
			DeleteEffekseerEffect(handleId_);
			handleId_ = -1;
		}
		break;

	case Resource::TYPE::LIVE2D:
		if (handleId_ != -1)
		{
			Live2D_DeleteModel(handleId_);
			handleId_ = -1;
		}
		break;

	case Resource::TYPE::FONT:
		for (const auto& variant : fontVariants_)
		{
			if (variant.handle != -1)
			{
				DeleteFontToHandle(variant.handle);
			}
		}
		fontVariants_.clear();
		handleId_ = -1;
		break;

	case Resource::TYPE::SOUND:
		if (handleId_ != -1)
		{
			DeleteSoundMem(handleId_);
			handleId_ = -1;
		}
		break;
	}
}

void Resource::CopyHandle(int* imgs) const
{

	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}

}

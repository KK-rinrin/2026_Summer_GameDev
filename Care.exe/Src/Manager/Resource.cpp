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
	thick_(-1)
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
	thick_(-1)
{
}

Resource::Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
	:
	type_(type),
	path_(path),
	numX_(numX),
	numY_(numY),
	sizeX_(sizeX),
	sizeY_(sizeY),
	handleId_(-1),
	handleIds_(nullptr),
	fontSize_(-1),
	thick_(-1)
{
}

Resource::Resource(TYPE type, const std::string& path, int fontSize, int thick)
	:
	type_(type),
	path_(path),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr),
	fontSize_(fontSize),
	thick_(thick)
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
		handleId_ = CreateFontToHandle(path_.c_str(), fontSize_, thick_, DX_FONTTYPE_ANTIALIASING);
		break;
	}

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
		if (handleId_ != -1)
		{
			DeleteFontToHandle(handleId_);
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

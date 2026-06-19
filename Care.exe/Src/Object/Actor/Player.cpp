#include "Player.h"
#include "ProcessMove.h"
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/ProgressManager.h"
#include "../../Utility/AsoUtility.h"
#include <algorithm>
#include <cmath>

Player::Player()
{
	if (ProgressManager::GetInstance().IsNurceCharExists())
	{
		processMove_ = new ProcessMove();
		processMove_->SetMoveSpeedPercent(INIT_MOVE_SPEED_PER);

	}
	else { processMove_ = nullptr; }

}

Player::~Player()
{
	if (processMove_ != nullptr)
	{
		delete processMove_;
		processMove_ = nullptr;
	}
}

void Player::Update(void)
{
	if (processMove_ != nullptr)
	{
		processMove_->Update(transform_);
	}

	// transform2D 側でパーセンテージのクランプとワールド座標への収め込みを行う
	transform_.Update();

	UpdateAnimation();

	anim_.Update();
}

void Player::SetLocalPercent(float x, float y)
{
	ActorBase::SetLocalPercent(x, y);
}

bool Player::IsFacingRight() const
{
	return ActorBase::IsFacingRight();
}

ProcessMove* Player::GetProcessMove(void)
{
	return processMove_;
}

void Player::Release(void)
{
	if (processMove_ != nullptr)
	{
		delete processMove_;
		processMove_ = nullptr;
	}
	ActorBase::Release();
}

void Player::InitLoad()
{
	const Resource& res = resMng_.Load(ResourceManager::SRC::PLAYER_GRAPHICS);

	// IMGS（分割画像）の場合は numX_ * numY_ 個の handleIds_ が存在する
	if (res.type_ == Resource::TYPE::IMGS && res.handleIds_ != nullptr)
	{
		int num = res.numX_ * res.numY_;
		std::vector<int> handles;
		handles.reserve(num);
		for (int i = 0; i < num; ++i) 
			handles.push_back(res.handleIds_[i]);
		transform_.SetHandles(handles,3,2);
	}
	else if (res.type_ == Resource::TYPE::IMG)
	{
		// 単一画像なら handleId_ を 1 要素の vector にして渡す
		transform_.SetHandles(std::vector<int>{ res.handleId_ },1,1);
	}
	else
	{
		// その他は空ベクタを渡す（安全側）
		transform_.SetHandles(std::vector<int>{},1,1);
	}
}

void Player::InitTransform()
{
	// 初期位置
	transform_.pos.x = INIT_PER.x;
	transform_.pos.y = INIT_PER.y;
	transform_.beforePos = transform_.pos;

}

void Player::InitCollider()
{
	radius_ = COL_RADIUS;
	collider_.radius = radius_;
}

void Player::InitAnimation()
{
	// 静止画像
	transform_.stillIndex = 1;
	transform_.stillIndex_back = 4;

	transform_.animController.SetPingPong(true);
}

void Player::InitPost()
{
}

void Player::UpdateAnimation()
{
	Transform2D& t = transform_;

	// スプライト分割が無ければ静止判定のみ
	if (t.indexX <= 1)
	{
		if (AsoUtility::Equals(t.beforePos, t.pos))
		{
			t.currentHandleIndex = AsoUtility::Clamp(t.stillIndex, 0, static_cast<int>(t.handleIds.size()) - 1);
			t.animController.Stop();
		}
		else
		{
			t.animController.Stop();
			t.currentHandleIndex = AsoUtility::Clamp(t.stillIndex, 0, static_cast<int>(t.handleIds.size()) - 1);
		}
		return;
	}

	// 移動量
	const float dx = t.pos.x - t.beforePos.x;
	const float dy = t.pos.y - t.beforePos.y;
	const float absdx = std::fabs(dx);
	const float absdy = std::fabs(dy);
	const float THRESH = 0.1f;

	bool isMoving = !(absdx < THRESH && absdy < THRESH);

	// 静止
	if (!isMoving)
	{
		t.animController.Stop();

		int still = t.stillIndex;
		t.currentHandleIndex = AsoUtility::Clamp(still, 0, static_cast<int>(t.handleIds.size()) - 1);
		return;
	}

	// 動いている → 再生
	t.animController.Play();
	t.animController.Update();

	// 方向推定（8方向）
	int deducedRot = t.rotDir;

	if (!(absdx < THRESH && absdy < THRESH))
	{
		constexpr float RAD_TO_DEG = 180.0f / 3.14159265f;
		float angleDeg = std::atan2(dy, dx) * RAD_TO_DEG;
		if (angleDeg < 0.0f) angleDeg += 360.0f;

		int sector = static_cast<int>((angleDeg + 22.5f) / 45.0f) % 8;

		const int sectorToRot[8] = { 2, 1, 0, 7, 6, 5, 4, 3 };
		deducedRot = sectorToRot[sector];
	}
	t.rotDir = deducedRot;

	// スプライト基点
	int framesPerDir = (t.indexX > 0) ? t.indexX : 1;
	int total = static_cast<int>(t.handleIds.size());

	int baseIndex = 0;
	bool flip = false;

	// 特殊ケース: 前後2セット
	if (framesPerDir == 3 && total == framesPerDir * 2)
	{
		bool verticalStrict = (absdx < THRESH && absdy >= THRESH);

		bool useBack = false;
		if (absdy >= THRESH)
		{
			useBack = (dy < 0.0f);
		}

		baseIndex = useBack ? framesPerDir : 0;

		if (verticalStrict)
		{
			const float screenCenterX = static_cast<float>(Application::SCREEN_SIZE_X) * 0.5f;
			flip = (t.GetWorldPos().x < screenCenterX);
		}
		else
		{
			flip = (dx < 0.0f);
		}
	}
	else if (t.indexY == 4 && total == framesPerDir * 2)
	{
		int half = total / 2;

		if (t.rotDir == 0 || t.rotDir == 3) baseIndex = 0;
		else baseIndex = half;

		flip = (t.rotDir == 3) || (t.rotDir == 2);
	}
	else
	{
		baseIndex = t.rotDir * framesPerDir;
		if (baseIndex < 0 || baseIndex >= total) baseIndex = 0;
		flip = false;
	}

	t.isLeft = flip;

	int frameOffset = t.animController.GetFrame();
	int target = baseIndex + frameOffset;
	t.currentHandleIndex = AsoUtility::Clamp(target, 0, total - 1);
}

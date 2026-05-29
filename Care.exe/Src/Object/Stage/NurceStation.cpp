#include "NurceStation.h"
#include "../../Manager/ResourceManager.h"

NurceStation::NurceStation()
	: pcHandle_(-1)
	, chairHandle_(-1)
	, lockerHandle_(-1)
{
}

NurceStation::~NurceStation()
{
}

void NurceStation::InitLoad()
{
	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_2).handleId_;

	pcHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_PC).handleId_;

	chairHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_CHAIR).handleId_;

	lockerHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_LOCKER).handleId_;
}

void NurceStation::InitTransform()
{
	AddTransformObject(Transform2D(pcHandle_, PC_POS, false));

	AddTransformObject(Transform2D(chairHandle_, CHAIR_POS, false));

	AddTransformObject(Transform2D(lockerHandle_, LOCKER_POS, false), LOCKER_BUFFER_Y);

}

void NurceStation::InitCollider()
{
	AddMBRectPercent({ 0.0f, 0.0f, 0.0f }, { 69.0f, 15.0f, 0.0f });	// PCé¸ï”

	// ÉçÉbÉJÅ[é¸ï”
	AddMBRectPercent({ 83.0f, 40.0f, 0.0f }, { 100.0f, 100.0f, 0.0f });
}

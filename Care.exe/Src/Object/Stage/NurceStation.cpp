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
	AddTransformObject(Transform2D(pcHandle_, PC_POS, false));

	chairHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_CHAIR).handleId_;
	AddTransformObject(Transform2D(chairHandle_, CHAIR_POS, false));

	lockerHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_LOCKER).handleId_;
	AddTransformObject(Transform2D(lockerHandle_, LOCKER_POS, false));
}

#include "PatientRoom.h"
#include "../../Manager/ResourceManager.h"

PatientRoom::PatientRoom()
	: wallHandle_(-1)
	, toiletHandle_(-1)
{

}

PatientRoom::~PatientRoom()
{

}

void PatientRoom::InitLoad()
{
	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_1).handleId_;
	wallHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_WALL).handleId_;
	toiletHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_TOILET).handleId_;

}

void PatientRoom::InitTransform()
{
	AddScreenObject(wallHandle_, WALL_SORT_Y, true);
	AddTransformObject(Transform2D(toiletHandle_, TOILET_POS, false));
}

void PatientRoom::InitCollider()
{
	AddMBRectPercent(WALL_BLOCK_LEFT_TOP, WALL_BLOCK_RIGHT_BOTTOM);
	AddMBRectPercent({ 75.0f, 5.0f, 0.0f }, { 87.0f, 10.0f, 0.0f });
}
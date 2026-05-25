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

void PatientRoom::InitTransform()
{

}

void PatientRoom::InitLoad()
{
	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_1).handleId_;

	wallHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_WALL).handleId_;
	AddScreenObject(wallHandle_, WALL_SORT_Y, true);
	AddMovementBlock(WALL_SORT_Y, WALL_THICKNESS);

	toiletHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_TOILET).handleId_;
	AddTransformObject(Transform2D(toiletHandle_, { 0.0f,0.0f,0.0f }, true));
}

void PatientRoom::UpdateProcess()
{
}
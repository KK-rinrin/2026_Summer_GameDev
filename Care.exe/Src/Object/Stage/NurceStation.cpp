#include "NurceStation.h"
#include "../../Manager/ResourceManager.h"

NurceStation::NurceStation()
{
}

NurceStation::~NurceStation()
{
}

void NurceStation::InitLoad()
{
	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_2).handleId_;
}
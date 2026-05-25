#pragma once
#include "StageBase.h"

class NurceStation : public StageBase
{
public:
	NurceStation();
	~NurceStation();

private:
	void InitLoad() override;
};
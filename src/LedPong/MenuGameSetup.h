#pragma once
#include <algorithm> // max, min
#include "GameConst.h"
#include "basic.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameConst.h"
#include "GameEnvironment.h"
#include "GameBase.h"
#include "MenuGameBase.h"

class MenuGameSetup : public MenuGameBase
{
public:
	MenuGameSetup(GameEnvironment* env);
protected:
	virtual GameBase* ButtonSelectLeft(int selectedItem);
	virtual GameBase* ButtonSelectRight(int selectedItem);
};


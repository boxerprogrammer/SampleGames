#pragma once
#include "Scene.h"
class ContinueScene : public Scene
{
private:
	int backH_;
	int frame_ = 60;
	
public:
	virtual void Update(Input&)override;
	virtual void Draw()override;
};


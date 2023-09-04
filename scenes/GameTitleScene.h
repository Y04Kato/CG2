#pragma once
#include "Iscene.h"
#include "Input.h"

class GameTitleScene :public Iscene{
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:

	Input* input_;
};

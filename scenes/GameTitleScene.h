#pragma once
#include "Iscene.h"
#include "input/Input.h"
#include "CJEngine.h"

class GameTitleScene :public Iscene{
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;
};

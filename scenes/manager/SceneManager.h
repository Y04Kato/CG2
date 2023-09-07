#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "input/Input.h"
#include "audio/Audio.h"
#include "utilities/GlobalVariables/GlobalVariables.h"

class SceneManager {
public:
	enum Secene {
		TITLE_SCENE,
		GAME_SCENE,
		SCENE_MAX
	};

	void Run();

private:
	CitrusJunosEngine* CJEngine_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::unique_ptr<Iscene>scene_[SCENE_MAX];

	void Initialize();
	void Update();
	void Finalize();
};
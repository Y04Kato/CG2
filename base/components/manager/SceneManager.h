#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "gamedata/scenes/GameTitleScene.h"
#include "gamedata/scenes/GamePlayScene.h"
#include "gamedata/scenes/GameClearScene.h"
#include "components/input/Input.h"
#include "components/audio/Audio.h"
#include "components/utilities/GlobalVariables/GlobalVariables.h"

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
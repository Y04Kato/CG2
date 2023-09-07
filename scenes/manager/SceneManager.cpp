#include"SceneManager.h"

void SceneManager::Run(){
	Initialize();
	Update();
	Finalize();
}

void SceneManager::Initialize() {
	const wchar_t kWindowTitle[] = { L"CG2_カトウ" };
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);
	//Engine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	CJEngine_->Initialize(kWindowTitle, 1280, 720);
	//DirectX
	dxCommon_ = DirectXCommon::GetInstance();
	//Audio
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//Input
	input_ = Input::GetInstance();
	input_->Initialize();

	//Scene
	scene_[TITLE_SCENE] = std::make_unique<GameTitleScene>();
	scene_[TITLE_SCENE]->Initialize();
	scene_[GAME_SCENE] = std::make_unique<GamePlayScene>();
	scene_[GAME_SCENE]->Initialize();

	Iscene::sceneNo = TITLE_SCENE;
}


void SceneManager::Update(){
	while (true) {
		//windowのメッセージを最優先で処理させる
		if (WinApp::GetInstance()->Procesmessage()) {
			break;
		}

		CJEngine_->BeginFrame();
		input_->Update();
		scene_[Iscene::sceneNo]->Update();
		scene_[Iscene::sceneNo]->Draw();
		CJEngine_->EndFrame();
	}
}

void SceneManager::Finalize(){
	CJEngine_->Finalize();
	audio_->Finalize();
	scene_[Iscene::sceneNo]->Finalize();
	CoUninitialize();
}
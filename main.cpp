#include "CJEngine.h"
#include "gameScene/GameScene.h"

const wchar_t kWindowTitle[] = { L"CG2_カトウ" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//COM初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	WinApp* win_ = nullptr;
	CitrusJunosEngine* CJEngine = new CitrusJunosEngine;
	CJEngine->Initialize(win_, kWindowTitle, 1280, 720);

	GameScene* gameScene = new GameScene();
	gameScene->Initialize(CJEngine, CJEngine->GetDirectXCommon());
	while (true) {
		//windowのメッセージを最優先で処理させる
		if (win_->Procesmessage()) {
			break;
		}
			//ゲームの処理
			CJEngine->BeginFrame();

			gameScene->Update();

			gameScene->Draw3D();
			gameScene->Draw2D();
			
			CJEngine->EndFrame();
	}

	//解放処理
	gameScene->Finalize();
	CJEngine->Finalize();
	CoUninitialize();
	return 0;
}
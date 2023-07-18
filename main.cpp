#include "MyEngine.h"
#include "GameScene.h"

const wchar_t kWindowTitle[] = { L"CG2_カトウ" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//COM初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	WinApp* win_ = nullptr;
	MyEngine* myEngine = new MyEngine;
	myEngine->Initialize(win_, kWindowTitle, 1280, 720);

	GameScene* gameScene = new GameScene();
	gameScene->Initialize(myEngine, myEngine->GetDirectXCommon());
	while (true) {
		//windowのメッセージを最優先で処理させる
		if (win_->Procesmessage()) {
			break;
		}
			//ゲームの処理
			myEngine->BeginFrame();

			gameScene->Update();

			gameScene->Draw();
			
			myEngine->EndFrame();
	}

	//解放処理
	gameScene->Finalize();
	myEngine->Finalize();
	CoUninitialize();
	return 0;
}
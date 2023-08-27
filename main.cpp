#include "CJEngine.h"
#include "gameScene/GameScene.h"

const wchar_t kWindowTitle[] = { L"CG2_カトウ" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakCheck;
	//COM初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	CitrusJunosEngine* CJEngine = new CitrusJunosEngine;
	CJEngine->Initialize(kWindowTitle, 1280, 720);

	GameScene* gameScene = new GameScene();
	gameScene->Initialize(CJEngine, CJEngine->GetDirectXCommon());
	while (true) {
		//windowのメッセージを最優先で処理させる
		if (WinApp::GetInstance()->Procesmessage()) {
			break;
		}
			//ゲームの処理
			CJEngine->BeginFrame();

			gameScene->Update();

			gameScene->Draw();
			
			CJEngine->EndFrame();
	}

	//解放処理
	gameScene->Finalize();
	delete gameScene;
	CJEngine->Finalize();
	delete CJEngine;
	CoUninitialize();
	return 0;
}
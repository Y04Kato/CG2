#include "MyEngine.h"
#include "DrawTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化
	WinApp* win_ = nullptr;
	MyEngine* Engine = new MyEngine;
	Engine->Initialization(win_, kWindowTitle,1280,720);

	Engine->variableInitialize();

	MSG msg{};
	//ウィンドウのxが押されるまでループ
	while (msg.message != WM_QUIT) {
		//windowのメッセージを最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理
			Engine->Update();
			Engine->BeginFrame();

			Engine->Draw();

			Engine->EndFrame();
		}
	}

	DirectXCommon::Release();

	return 0;
}
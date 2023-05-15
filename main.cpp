#include "Win.h"

const wchar_t kWindowTitle[] = { L"CG2" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化
	winClear::CreateWindowView(kWindowTitle);

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


		}
	}

	return 0;
}
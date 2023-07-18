#include "MyEngine.h"
#include "CreateTriangle.h"

const wchar_t kWindowTitle[] = { L"CG2_カトウ" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//COM初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	WinApp* win_ = nullptr;
	MyEngine* myEngine = new MyEngine;
	myEngine->Initialization(win_, kWindowTitle, 1280, 720);

	myEngine->Initialize();

	Vector4 data1 = { -0.5f,-0.5f,0.0f,1.0f };
	Vector4 data2 = { 0.0f,0.5f,0.0f,1.0f };
	Vector4 data3 = { 0.5f,-0.5f,0.0f,1.0f };
	Vector4 material1 = { 1.0f,0.0f,0.0f,1.0f };

	Vector4 data4 = { -0.5f,-0.5f,0.5f,1.0f };
	Vector4 data5 = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 data6 = { 0.5f,-0.5f,-0.5f,1.0f };
	Vector4 material2 = { 0.0f,1.0f,0.0f,1.0f };

	/*Vector4 data1 = { -0.2f,-0.2f,0.0f,1.0f };
	Vector4 data2 = { 0.0f,0.2f,0.0f,1.0f };
	Vector4 data3 = { 0.2f,-0.2f,0.0f,1.0f };
	Vector4 material1 = { 1.0f,0.0f,0.0f,1.0f };

	Vector4 data4 = { -0.8f,-0.8f,0.0f,1.0f };
	Vector4 data5 = { -0.6f,-0.4f,0.0f,1.0f };
	Vector4 data6 = { -0.4f,-0.8f,0.0f,1.0f };
	Vector4 material2 = { 0.0f,1.0f,0.0f,1.0f };

	Vector4 data7 = { 0.4f,-0.8f,0.0f,1.0f };
	Vector4 data8 = { 0.6f,-0.4f,0.0f,1.0f };
	Vector4 data9 = { 0.8f,-0.8f,0.0f,1.0f };
	Vector4 material3 = { 0.0f,0.0f,1.0f,1.0f };*/

	while (true) {
		//windowのメッセージを最優先で処理させる
		if (win_->Procesmessage()) {
			break;
		}
			//ゲームの処理
			myEngine->BeginFrame();

			myEngine->Update();

			//三角形描画
			myEngine->DrawTriangle(data1 ,data2, data3,material1);

			myEngine->DrawTriangle(data4, data5, data6, material2);

			//myEngine->DrawTriangle(data7, data8, data9, material3);

			
			myEngine->EndFrame();
	}

	//解放処理
	myEngine->Finalize();
	CoUninitialize();
	return 0;
}
#pragma once
#include <Windows.h>
#include <cstdint>

class winClear {
public:
	//クライアント領域サイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static void CreateWindowView();

private:
	static inline	WNDCLASS wc{};// ウィンドウクラス
	static	HWND hwnd;
};

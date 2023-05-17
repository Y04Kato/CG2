#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp {
public:
	//クライアント領域サイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() const { return hwnd_; }

	static void CreateWindowView(const wchar_t* title);

private:
	static inline	WNDCLASS wc_{};// ウィンドウクラス
	static	HWND hwnd_;
};

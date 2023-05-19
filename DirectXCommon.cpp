#include "DirectXCommon.h"
#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <dxgidebug.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Winmm.lib")

void DirectXCommon::Initialization(WinApp* win, const wchar_t* title, int32_t backBufferWidth, int32_t backBufferHeight) {

	winApp_ = win;
	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	winApp_->CreateWindowView(L"CG2", 1280, 720);
	// DXGIデバイス初期化
	InitializeDXGIDevice();

	// コマンド関連初期化
	InitializeCommand();

	// スワップチェーンの生成
	CreateSwapChain();

	// レンダーターゲット生成
	CreateFinalRenderTargets();

	// フェンス生成
	CreateFence();
}

void DirectXCommon::InitializeDXGIDevice() {
	HRESULT result = S_FALSE;

	//DXGIファクトリーの生成
	dxgiFactory_ = nullptr;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	//使用するアダプタ用の変数
	useAdapter_ = nullptr;
	//順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) !=
		DXGI_ERROR_NOT_FOUND; i++) {
		//アダプター情報の取得
		DXGI_ADAPTER_DESC3 adapterDesc{};
		result = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(result));

		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用アダプタの情報をログに出力
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter_ = nullptr;//ソフトウェアアダプタの場合は無視
	}
	//適切なアダプタがないため起動しない
	assert(useAdapter_ != nullptr);

	//D3D12Deviceの生成
	device_ = nullptr;
	//機能レベルとログ出力
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelString[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか確認
	for (size_t i = 0; i < _countof(featureLevels); i++) {
		//採用したアダプターでデバイス生成
		result = D3D12CreateDevice(useAdapter_, featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルのデバイス生成に成功したか確認
		if (SUCCEEDED(result)) {
			//生成できたのでログ出力をしてループ抜け
			Log(std::format("FeatureLevel : {}\n", featureLevelString[i]));
			break;
		}
	}

	//デバイス生成失敗の為起動しない
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!\n");//初期化完了ログ

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//ヤバイエラーで止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラーで止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するメッセージID
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//解放
		infoQueue->Release();
	}
#endif // _DEBUG
}

void DirectXCommon::InitializeCommand() {
	HRESULT result = S_FALSE;

	commandQueue_ = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成失敗の為、起動しない
	assert(SUCCEEDED(result));

	//コマンドアロケータの生成
	commandAllocator_ = nullptr;
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	//コマンドリストを生成する
	commandList_ = nullptr;
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_, nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリスト生成失敗の為、起動しない
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateSwapChain() {
	HRESULT result = S_FALSE;

	//スワップチェーン
	swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WinApp::kClientWidth;//画面の幅
	swapChainDesc.Height = WinApp::kClientHeight;//画面の高さ
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用
	swapChainDesc.BufferCount = 2;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタに移したら中身を破棄

	//コマンドキュー、ウィンドウハンドル、設定を渡して生成
	result = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_, winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain_));
	assert(SUCCEEDED(result));

	//ディスクリプタヒープの生成
	rtvDescriptorHeap_ = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;//ダブルバッファ用に２つ　多くても構わない
	result = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap_));
	//ディスクリプタヒープが生成失敗の為、起動しない
	assert(SUCCEEDED(result));

	//SwapChainからResourceを引っ張ってくる
	swapChainResources_[0] = { nullptr };
	swapChainResources_[1] = { nullptr };
	result = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	//取得できなければ起動しない
	assert(SUCCEEDED(result));

	result = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateFinalRenderTargets() {
	HRESULT result = S_FALSE;

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力情報をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2Drテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	//RTVを2つ作る為、ディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//まず一つ目、最初の場所に作る、作る場所は指定が必要
	rtvHandles[0] = rtvStartHandle;
	device_->CreateRenderTargetView(swapChainResources_[0], &rtvDesc, rtvHandles[0]);
	//2つ目のディスクリプタハンドルを得る
	rtvHandles[1].ptr = rtvHandles[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//２つ目を生成
	device_->CreateRenderTargetView(swapChainResources_[1], &rtvDesc, rtvHandles[1]);
}

void DirectXCommon::CreateFence() {
	HRESULT result = S_FALSE;

	//初期値0でFenceを作る
	fence_ = nullptr;
	fenceValue_ = 0;
	result = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));

	//Fenceのsignalを待つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, false, false, NULL);
	assert(fenceEvent_ != nullptr);
}

void DirectXCommon::PreDraw() {
	//書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のbarrierはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにする
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//barrier対象のリソース、バックばっがに対して行う
	barrier.Transition.pResource = swapChainResources_[backBufferIndex];
	//遷移前のresourcestate
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のresourcestate
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);
	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, nullptr);
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色、RGBA順
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);
}

void DirectXCommon::PostDraw() {
	HRESULT result = S_FALSE;

	//画面描画処理の終わり、状態を遷移
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);
	//コマンドリストの内容を確定させる。全てのコマンドを積んでからcloseする
	result = commandList_->Close();
	assert(SUCCEEDED(result));

	//GPUにコマンドリストを実行させる
	ID3D12CommandList* commandLists[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うよう通知する
	swapChain_->Present(1, 0);
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまで辿り着いた時、Fenceの値を指定した値に代入するようにsignalを送る
	commandQueue_->Signal(fence_, fenceValue_);

	//次のフレーム用のコマンドリストを準備
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_, nullptr);
	assert(SUCCEEDED(result));
}

void DirectXCommon::Release() {
	//解放処理
	CloseHandle(fenceEvent_);
	fence_->Release();
	rtvDescriptorHeap_->Release();
	swapChainResources_[0]->Release();
	swapChainResources_[1]->Release();
	swapChain_->Release();
	commandList_->Release();
	commandAllocator_->Release();
	commandQueue_->Release();
	device_->Release();
	useAdapter_->Release();
	dxgiFactory_->Release();
#ifdef _DEBUG
	winApp_->GetdebugController()->Release();
#endif // _DEBUG
	CloseWindow(winApp_->GetHwnd());

	////リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}

WinApp* winApp_;

//DXGIファクトリーの生成
IDXGIFactory7* dxgiFactory_;

//使用するアダプタ用の変数
IDXGIAdapter4* useAdapter_;

//D3D12Deviceの生成
ID3D12Device* device_;

//コマンドキュー生成
ID3D12CommandQueue* commandQueue_;

//コマンドアロケータの生成
ID3D12CommandAllocator* commandAllocator_;

//コマンドリストを生成する
ID3D12GraphicsCommandList* commandList_;

//スワップチェーン
IDXGISwapChain4* swapChain_;

//ディスクリプタヒープの生成
ID3D12DescriptorHeap* rtvDescriptorHeap_;

//RTVを２つ作るのでディスクリプタを２つ用意
D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
ID3D12Resource* swapChainResources_[2];

//Fence
ID3D12Fence* fence_;
UINT64 fenceValue_;
HANDLE fenceEvent_;

int32_t backBufferWidth_;
int32_t backBufferHeight_;

static HRESULT hr_;
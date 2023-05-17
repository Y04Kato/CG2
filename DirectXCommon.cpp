#include "DirectXCommon.h"

void DirectXCommon::Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

std::wstring ConvertString(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str)
{
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

void DirectXCommon::DirectXInitialization() {

	InitializeDXGIDevice();
	InitializeCommand();
	CreateSwapChain();
	CreateFinalRenderTargets();

}

void DirectXCommon::InitializeDXGIDevice() {
	HRESULT result = S_FALSE;

#ifdef _DEBUG//デバッグレイヤー
	debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効化
		debugController->EnableDebugLayer();
		//GPU側でもチェックを行う
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	//DXGIファクトリーの生成
	dxgiFactory = nullptr;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//使用するアダプタ用の変数
	useAdapter = nullptr;
	//順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++) {
		//アダプター情報の取得
		DXGI_ADAPTER_DESC3 adapterDesc{};
		result = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(result));

		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用アダプタの情報をログに出力
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;//ソフトウェアアダプタの場合は無視
	}
	//適切なアダプタがないため起動しない
	assert(useAdapter != nullptr);

	//D3D12Deviceの生成
	device = nullptr;
	//機能レベルとログ出力
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelString[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか確認
	for (size_t i = 0; i < _countof(featureLevels); i++) {
		//採用したアダプターでデバイス生成
		result = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
		//指定した機能レベルのデバイス生成に成功したか確認
		if (SUCCEEDED(result)) {
			//生成できたのでログ出力をしてループ抜け
			Log(std::format("FeatureLevel : {}\n", featureLevelString[i]));
			break;
		}
	}

	//デバイス生成失敗の為起動しない
	assert(device != nullptr);
	Log("Complete create D3D12Device!!\n");//初期化完了ログ

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
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

	commandQueue = nullptr;
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成失敗の為、起動しない
	assert(SUCCEEDED(result));

	//コマンドアロケータの生成
	commandAllocator = nullptr;
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	//コマンドリストを生成する
	commandList = nullptr;
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	//コマンドリスト生成失敗の為、起動しない
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateSwapChain() {
	HRESULT result = S_FALSE;

	//スワップチェーン
	swapChain = nullptr;
	swapChainDesc.Width = kClientWidth;//画面の幅
	swapChainDesc.Height = kClientHeight;//	画面の高さ
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用
	swapChainDesc.BufferCount = 2;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタに移したら中身を破棄

	//コマンドキュー、ウィンドウハンドル、設定を渡して生成
	result = dxgiFactory->CreateSwapChainForHwnd(commandQueue, winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(result));

	//ディスクリプタヒープの生成
	rtvDescriptorHeap = nullptr;
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;//ダブルバッファ用に２つ　多くても構わない
	result = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプタヒープが生成失敗の為、起動しない
	assert(SUCCEEDED(result));

	//SwapChainからResourceを引っ張ってくる
	swapChainResources[0] = { nullptr };
	swapChainResources[1] = { nullptr };
	result = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	//取得できなければ起動しない
	assert(SUCCEEDED(result));

	result = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateFinalRenderTargets() {
	HRESULT result = S_FALSE;

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力情報をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2Drテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//RTVを2つ作る為、ディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//まず一つ目、最初の場所に作る、作る場所は指定が必要
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0], &rtvDesc, rtvHandles[0]);
	//2つ目のディスクリプタハンドルを得る
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//２つ目を生成
	device->CreateRenderTargetView(swapChainResources[1], &rtvDesc, rtvHandles[1]);

	//書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のbarrierはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにする
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//barrier対象のリソース、バックばっがに対して行う
	barrier.Transition.pResource = swapChainResources[backBufferIndex];
	//遷移前のresourcestate
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のresourcestate
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
	//描画先のRTVを設定する
	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, nullptr);
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色、RGBA順
	commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);
	//画面描画処理の終わり、状態を遷移
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
	//コマンドリストの内容を確定させる。全てのコマンドを積んでからcloseする
	result = commandList->Close();
	assert(SUCCEEDED(result));

	CreateFence();

	//GPUにコマンドリストを実行させる
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うよう通知する
	swapChain->Present(1, 0);
	//Fenceの値を更新
	fenceValue++;
	//GPUがここまで辿り着いた時、Fenceの値を指定した値に代入するようにsignalを送る
	commandQueue->Signal(fence, fenceValue);

	//次のフレーム用のコマンドリストを準備
	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));
	result = commandList->Reset(commandAllocator, nullptr);
	assert(SUCCEEDED(result));

	//Fenceの値が指定したsignal値に辿り着いているか確認する
	if (fence->GetCompletedValue() < fenceValue) {
		//指定したsignalに辿り着いていないので、辿り着くまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void DirectXCommon::CreateFence() {
	HRESULT result = S_FALSE;

	//初期値0でFenceを作る
	fence = nullptr;
	fenceValue = 0;
	result = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));

	//Fenceのsignalを待つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, false, false, NULL);
	assert(fenceEvent != nullptr);
}

void DirectXCommon::Release() {
	//解放処理
	CloseHandle(fenceEvent);
	fence->Release();
	rtvDescriptorHeap->Release();
	swapChainResources[0]->Release();
	swapChainResources[1]->Release();
	swapChain->Release();
	commandList->Release();
	commandAllocator->Release();
	commandQueue->Release();
	device->Release();
	useAdapter->Release();
	dxgiFactory->Release();
#ifdef _DEBUG
	debugController->Release();
#endif // _DEBUG
	CloseWindow(winApp_->GetHwnd());

	ResourceCheck();
}

void DirectXCommon::ResourceCheck() {
	//リソースチェック
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}

WinApp* DirectXCommon::winApp_;

//DXGIファクトリーの生成
IDXGIFactory7* DirectXCommon::dxgiFactory;
HRESULT DirectXCommon::result;

//使用するアダプタ用の変数
IDXGIAdapter4* DirectXCommon::useAdapter;

//D3D12Deviceの生成
ID3D12Device* DirectXCommon::device;

//コマンドキュー生成
ID3D12CommandQueue* DirectXCommon::commandQueue;
//コマンドアロケータの生成
ID3D12CommandAllocator* DirectXCommon::commandAllocator;
//コマンドリストを生成する
ID3D12GraphicsCommandList* DirectXCommon::commandList;

//スワップチェーン
IDXGISwapChain4* DirectXCommon::swapChain;

//SwapChainからResourceを引っ張ってくる
ID3D12Resource* DirectXCommon::swapChainResources[2];

//ディスクリプタヒープの生成
ID3D12DescriptorHeap* DirectXCommon::rtvDescriptorHeap;

//Fence
ID3D12Fence* DirectXCommon::fence;
uint64_t DirectXCommon::fenceValue;
HANDLE DirectXCommon::fenceEvent;

//debug
ID3D12Debug1* DirectXCommon::debugController;
IDXGIDebug1* DirectXCommon::debug;
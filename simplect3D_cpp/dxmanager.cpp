#include <si3_DxManager.h>
#include <si3_popular.h>
#include <si3_utility.h>

namespace si3
{
	bool DxManager::init(HWND win_hand, unsigned int screen_width, unsigned int screen_height)
	{
		// デバイス情報テーブル
		struct TCreateDevice {
			D3DDEVTYPE	type;			// デバイスの種類
			DWORD		behavior;		// デバイスの動作
		};
		const int max_device_num = 3;
		const TCreateDevice device[max_device_num] = {
				{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// ハードウェアシェーダーを使う
				{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
				{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
		};
		int		device_kind_counter;			// デバイス種別カウンタ

		// Direct3D オブジェクトを生成
		if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		//	MessageBox(hWnd, _T("Direct3Dオブジェクトの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
			return false;
		}
		// Direct3D 初期化パラメータの設定
		ZeroMemory(&present_parameters, sizeof(present_parameters));
		present_parameters.BackBufferWidth = screen_width;				// ゲーム画面サイズ
		present_parameters.BackBufferHeight = screen_height;
		present_parameters.Windowed = true;					// ウィンドウモード
		present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		present_parameters.EnableAutoDepthStencil = TRUE;						// Zバッファ有効
		present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;				// Zバッファフォーマット
		if (present_parameters.Windowed) {
			// ウィンドウモード
			present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;	// バックバッファ
			present_parameters.FullScreen_RefreshRateInHz = 0;					// リフレッシュレート
			present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// 垂直同期を待たない
		}
#if 0
		else {
			// フルスクリーンモード
			present_parameters.BackBufferFormat = D3DFMT_X8R8G8B8;	// バックバッファ
			present_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
			present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// 垂直同期を待つ
		}
#endif

		// デバイスオブジェクトを作成
		for (device_kind_counter = 0; device_kind_counter < max_device_num; device_kind_counter++) {
			HRESULT hr = d3d->CreateDevice(
				D3DADAPTER_DEFAULT,
				device[device_kind_counter].type,
				win_hand,
				device[device_kind_counter].behavior,
				&present_parameters,
				&dxdevice);

			if (SUCCEEDED(hr)) break;
		}
		if (device_kind_counter >= max_device_num) {
		//	MessageBox(hWnd, _T("デバイスの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
			return false;
		}

		// マテリアルカラー使用
		dxdevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		dxdevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		dxdevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
		dxdevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

#if 0
#ifdef _DEBUG
		// デバッグ用フォントを設定
		D3DXCreateFont(dxdevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pFont);
#endif
#endif

		return true;
	}


	D3DPRESENT_PARAMETERS * DxManager::get_present_parameters()
	{
		return &present_parameters;
	}

	LPDIRECT3DDEVICE9 DxManager::get_dxdevice()
	{
		return dxdevice;
	}

	const D3DPRESENT_PARAMETERS * DxManager::get_present_parameters() const
	{
		return &present_parameters;
	}

	const LPDIRECT3DDEVICE9 DxManager::get_dxdevice() const
	{
		return dxdevice;
	}



	DxManager::DxManager()
	{
		dxdevice = nullptr;
		d3d = nullptr;
	}

	DxManager::~DxManager()
	{
		dxsaferelease(dxdevice);
		dxsaferelease(d3d);
	}
}
#include <dxmanager.h>
#include <popular.h>
#include <utility.h>

namespace si3
{
	bool dxmanager::init(HWND hWnd, unsigned int screen_width, unsigned int screen_height)
	{
		// デバイス情報テーブル
		struct TCreateDevice {
			D3DDEVTYPE	type;			// デバイスの種類
			DWORD		behavior;		// デバイスの動作
		};
		const int c_nMaxDevice = 3;
		const TCreateDevice device[c_nMaxDevice] = {
				{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// ハードウェアシェーダーを使う
				{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
				{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
		};
		int		nDev;			// デバイス種別カウンタ

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
		for (nDev = 0; nDev < c_nMaxDevice; nDev++) {
			HRESULT hr = d3d->CreateDevice(
				D3DADAPTER_DEFAULT,
				device[nDev].type,
				hWnd,
				device[nDev].behavior,
				&present_parameters,
				&dxdevice);

			if (SUCCEEDED(hr)) break;
		}
		if (nDev >= c_nMaxDevice) {
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


	D3DPRESENT_PARAMETERS * dxmanager::get_present_parameters()
	{
		return &present_parameters;
	}

	LPDIRECT3DDEVICE9 dxmanager::get_dxdevice()
	{
		return dxdevice;
	}

	const D3DPRESENT_PARAMETERS * dxmanager::get_present_parameters() const
	{
		return &present_parameters;
	}

	const LPDIRECT3DDEVICE9 dxmanager::get_dxdevice() const
	{
		return dxdevice;
	}



	dxmanager::dxmanager()
	{
		dxdevice = nullptr;
		d3d = nullptr;
	}

	dxmanager::~dxmanager()
	{
		saferelease(dxdevice);
		saferelease(d3d);
	}
}
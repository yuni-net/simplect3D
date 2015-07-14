#include <display_manager.h>
#include <utility.h>

namespace si3
{
	DisplayManager::DisplayManager()
	{
		context_color(1.0f, 1.0f, 1.0f);
	}

	void DisplayManager::init(D3DPRESENT_PARAMETERS * present_parameters, LPDIRECT3DDEVICE9 dxdevice)
	{
		this->present_parameters = present_parameters;
		this->dxdevice = dxdevice;

		dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		dxdevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		dxdevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		dxdevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	}

	void DisplayManager::register_display_object(const display_object & display_objecti)
	{
		display_objects.add(&display_objecti);
	}


	void DisplayManager::context_color(float r, float g, float b)
	{
		red_ = r;
		green_ = g;
		blue_ = b;
	}

	float DisplayManager::context_red() const
	{
		return red_;
	}
	float DisplayManager::context_green() const
	{
		return green_;
	}
	float DisplayManager::context_blue() const
	{
		return blue_;
	}



	void DisplayManager::draw()
	{
		// ビューポートをクリア
		dxdevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			D3DCOLOR_XRGB(round_uchar(255 * red_), round_uchar(255 * green_), round_uchar(255 * blue_)),
			1.0f,
			0);

		if (SUCCEEDED(dxdevice->BeginScene())) {	// シーン開始

			for (unsigned int object_No = 0; object_No < display_objects.size(); ++object_No)
			{
				display_objects[object_No]->draw_no_alpha();
			}

			for (unsigned int object_No = 0; object_No < display_objects.size(); ++object_No)
			{
				display_objects[object_No]->draw_alpha();
			}

			dxdevice->EndScene();					// シーン終了
		}

		// バックバッファに描画した内容を表示
		if (FAILED(dxdevice->Present(NULL, NULL, NULL, NULL))) {
			dxdevice->Reset(present_parameters);
		}

		display_objects.zerosize();

		register_display_object(skydome);
	}

	sky_dome_mini & DisplayManager::get_skydome()
	{
		return skydome;
	}

}
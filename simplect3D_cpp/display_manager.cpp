#include <display_manager.h>
#include <utility.h>

namespace si3
{
	display_manager::display_manager()
	{
		context_color(1.0f, 1.0f, 1.0f);
	}

	void display_manager::init(D3DPRESENT_PARAMETERS * present_parameters, LPDIRECT3DDEVICE9 dxdevice)
	{
		this->present_parameters = present_parameters;
		this->dxdevice = dxdevice;

		dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		dxdevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		dxdevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		dxdevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	}

	void display_manager::register_display_object(const display_object & display_objecti)
	{
		display_objects.add(&display_objecti);
	}


	void display_manager::context_color(float r, float g, float b)
	{
		fRed_ = r;
		fGreen_ = g;
		fBlue_ = b;
	}

	float display_manager::context_red() const
	{
		return fRed_;
	}
	float display_manager::context_green() const
	{
		return fGreen_;
	}
	float display_manager::context_blue() const
	{
		return fBlue_;
	}



	void display_manager::draw()
	{
		// �r���[�|�[�g���N���A
		dxdevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			D3DCOLOR_XRGB(round_uchar(255 * fRed_), round_uchar(255 * fGreen_), round_uchar(255 * fBlue_)),
			1.0f,
			0);

		if (SUCCEEDED(dxdevice->BeginScene())) {	// �V�[���J�n

			for (unsigned int object_No = 0; object_No < display_objects.size(); ++object_No)
			{
				display_objects[object_No]->draw_no_alpha();
			}

			for (unsigned int object_No = 0; object_No < display_objects.size(); ++object_No)
			{
				display_objects[object_No]->draw_alpha();
			}

			dxdevice->EndScene();					// �V�[���I��
		}

		// �o�b�N�o�b�t�@�ɕ`�悵�����e��\��
		if (FAILED(dxdevice->Present(NULL, NULL, NULL, NULL))) {
			dxdevice->Reset(present_parameters);
		}

		display_objects.zerosize();

		register_display_object(skydome);
	}

	sky_dome_mini & display_manager::get_skydome()
	{
		return skydome;
	}

}
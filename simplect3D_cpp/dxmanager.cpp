#include <dxmanager.h>
#include <popular.h>
#include <utility.h>

namespace si3
{
	bool dxmanager::init(HWND hWnd, unsigned int screen_width, unsigned int screen_height)
	{
		// �f�o�C�X���e�[�u��
		struct TCreateDevice {
			D3DDEVTYPE	type;			// �f�o�C�X�̎��
			DWORD		behavior;		// �f�o�C�X�̓���
		};
		const int c_nMaxDevice = 3;
		const TCreateDevice device[c_nMaxDevice] = {
				{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// �n�[�h�E�F�A�V�F�[�_�[���g��
				{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
				{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
		};
		int		nDev;			// �f�o�C�X��ʃJ�E���^

		// Direct3D �I�u�W�F�N�g�𐶐�
		if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		//	MessageBox(hWnd, _T("Direct3D�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
			return false;
		}
		// Direct3D �������p�����[�^�̐ݒ�
		ZeroMemory(&present_parameters, sizeof(present_parameters));
		present_parameters.BackBufferWidth = screen_width;				// �Q�[����ʃT�C�Y
		present_parameters.BackBufferHeight = screen_height;
		present_parameters.Windowed = true;					// �E�B���h�E���[�h
		present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		present_parameters.EnableAutoDepthStencil = TRUE;						// Z�o�b�t�@�L��
		present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;				// Z�o�b�t�@�t�H�[�}�b�g
		if (present_parameters.Windowed) {
			// �E�B���h�E���[�h
			present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@
			present_parameters.FullScreen_RefreshRateInHz = 0;					// ���t���b�V�����[�g
			present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// ����������҂��Ȃ�
		}
#if 0
		else {
			// �t���X�N���[�����[�h
			present_parameters.BackBufferFormat = D3DFMT_X8R8G8B8;	// �o�b�N�o�b�t�@
			present_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
			present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// ����������҂�
		}
#endif

		// �f�o�C�X�I�u�W�F�N�g���쐬
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
		//	MessageBox(hWnd, _T("�f�o�C�X�̍쐬�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
			return false;
		}

		// �}�e���A���J���[�g�p
		dxdevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		dxdevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		dxdevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
		dxdevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

#if 0
#ifdef _DEBUG
		// �f�o�b�O�p�t�H���g��ݒ�
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
#include "si3_SoundManager.h"
#include "si3_saferelease.h"

namespace si3
{


	void SoundManager::reserve4ID_path(int reserve_size)
	{
		idpath_list.requre(reserve_size);
	}
	void SoundManager::register_ID_path(uint id, const char * path)
	{
		idpath_list.reqsize(id + 1);
		idpath_list[id] = path;
	}




	const char * SoundManager::get_path(uint id) const
	{
		return idpath_list[id];
	}


	bool SoundManager::init()
	{
		window_.invisible().create();	// �����E�B���h�E�ɂ��Ă�������

		HRESULT hr;

		// IDirectSound8�C���^�[�t�F�C�X�̎擾
		hr = DirectSoundCreate8(NULL, &dxs, NULL);
		if (FAILED(hr))
		{
			fw::popup("IDirectSound8�C���^�[�t�F�C�X�̎擾�Ɏ��s");
			return false;
		}

		//�����ŋ������x����ݒ�
		hr = dxs->SetCooperativeLevel(window_.myhandle(), DSSCL_PRIORITY);
		if (FAILED(hr))
		{
			// �������x���̐ݒ�Ɏ��s
			fw::popup("�������x���̐ݒ�Ɏ��s");
			return false;
		}

		// �v���C�}���E�o�b�t�@�̍쐬
		// DSBUFFERDESC�\���̂�ݒ�
		DSBUFFERDESC dsbdesc;
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		// �v���C�}���E�o�b�t�@���w��
		dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwBufferBytes = 0;
		dsbdesc.lpwfxFormat = NULL;

		// �o�b�t�@�����
		LPDIRECTSOUNDBUFFER primarysb;
		hr = dxs->CreateSoundBuffer(&dsbdesc, &primarysb, NULL);
		if (FAILED(hr))
		{
			// �o�b�t�@�̍쐬�Ɏ��s
			fw::popup("�v���C�}���E�o�b�t�@�̍쐬�Ɏ��s");
			return false;
		}

		// �v���C�}���E�o�b�t�@��Wave�t�H�[�}�b�g��ݒ�
		// �@�@�@�D�拦�����x���ȏ�̋������x�����ݒ肳��Ă���K�v������܂��D
		WAVEFORMATEX pcmwf;
		ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX));
		pcmwf.wFormatTag = WAVE_FORMAT_PCM;
		pcmwf.nChannels = 2;			// �Q�`�����l���i�X�e���I�j
		pcmwf.nSamplesPerSec = 44100;	// �T���v�����O�E���[�g�@44.1kHz
		pcmwf.nBlockAlign = 4;
		pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
		pcmwf.wBitsPerSample = 16;		// 16�r�b�g
		hr = primarysb->SetFormat(&pcmwf);
		if (FAILED(hr))
		{
			fw::popup("�v���C�}���E�o�b�t�@�̃t�H�[�}�b�g�ݒ�Ɏ��s");
			return false;
		}

		return true;
	}

	LPDIRECTSOUND8 SoundManager::get_interface() const
	{
		return dxs;
	}


	SoundManager::~SoundManager()
	{
		dxsaferelease(primarysb);
		dxsaferelease(dxs);
	}


}
#include "sound_data.h"
#include "sound_manager.h"
#include "Manager.h"
#include "saferelease.h"

namespace si3
{

	bool sound_data::load(Manager & si3m, uint uID)
	{
		this->uID = uID;
		si3mp = &si3m;

		sound_manager & soundm = si3m.get_sound_manager();
		const char * path = soundm.get_path(uID);

		// WAVE�t�@�C�����J��
		if (wav_data.load(path) == false)
		{
			fw::popup("failed to load Wave File");
			return false;
		}

		// �Z�J���_���E�o�b�t�@���쐬����
		// WAVEFORMATEX�\���̂�ݒ�
		wfe.wFormatTag = wav_data.format_tag();
		wfe.nChannels = wav_data.channels();
		wfe.nSamplesPerSec = wav_data.samples_per_sec();
		wfe.nAvgBytesPerSec = wav_data.avg_bytes_per_sec();
		wfe.nBlockAlign = wav_data.block_align();
		wfe.wBitsPerSample = wav_data.bits_per_sample();
		wfe.cbSize = wav_data.pwf_size();

		// DSBUFFERDESC�\���̂�ݒ�
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
			DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
		dsbdesc.dwBufferBytes = wav_data.size();
		dsbdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
		dsbdesc.lpwfxFormat = &wfe;



		return true;
	}

	uint sound_data::getID() const
	{
		return uID;
	}


	bool sound_data::create_buffer(IDirectSoundBuffer8 * & secondarysb) const
	{
		HRESULT hr;

		sound_manager & soundm = si3mp->get_sound_manager();

		// �o�b�t�@�����
		LPDIRECTSOUNDBUFFER pDSB;
		hr = soundm.get_interface()->CreateSoundBuffer(&dsbdesc, &pDSB, NULL);
		if (FAILED(hr))
		{
			fw::popup("failed to secondary buffer");
			return false;
		}
		hr = pDSB->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID *>(&secondarysb));
		dxsaferelease(pDSB);
		if (FAILED(hr))
		{
			fw::popup("IDirectSoundBuffer8�C���^�[�t�F�C�X�̎擾�Ɏ��s");
			return false;
		}

		// �Z�J���_���E�o�b�t�@��Wave�f�[�^����������
		LPVOID lpvPtr1; 	// �ŏ��̃u���b�N�̃|�C���^
		DWORD dwBytes1; 	// �ŏ��̃u���b�N�̃T�C�Y
		LPVOID lpvPtr2; 	// �Q�Ԗڂ̃u���b�N�̃|�C���^
		DWORD dwBytes2; 	// �Q�Ԗڂ̃u���b�N�̃T�C�Y

		hr = secondarysb->Lock(0, wav_data.size(), &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);

		// DSERR_BUFFERLOST���Ԃ��ꂽ�ꍇ�CRestore���\�b�h���g���ăo�b�t�@�𕜌�����
		if (DSERR_BUFFERLOST == hr)
		{
			secondarysb->Restore();
			hr = secondarysb->Lock(0, wav_data.size(), &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
		}
		if (FAILED(hr))
		{
			fw::popup("failed to lock secondary buffer");
			return false;
		}

		// ���b�N����

		// �����ŁC�o�b�t�@�ɏ�������
		// �o�b�t�@�Ƀf�[�^���R�s�[����
		memcpy(lpvPtr1, wav_data.data(), dwBytes1);
		if (0 != dwBytes2) memcpy(lpvPtr2, fw::pointer_cast<const char *>(wav_data.data()) + dwBytes1, dwBytes2);

		// �������݂��I������炷����Unlock����D
		hr = secondarysb->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
		if (FAILED(hr)) return false;

		return true;
	}




	sound_data::sound_data()
	{
		construct();
	}
	sound_data::sound_data(Manager & si3m, uint uID)
	{
		construct();
		load(si3m, uID);
	}


	void sound_data::construct()
	{
		// Nothing
	}

}
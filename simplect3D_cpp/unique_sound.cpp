#include "unique_sound.h"
#include "Manager.h"
#include "saferelease.h"

namespace si3
{
	void unique_sound::construct()
	{
		pSoundData = 0;	// NULL
		ownership = false;
		secondarysb = nullptr;
	}

	unique_sound::unique_sound()
	{
		construct();
	}
	unique_sound::unique_sound(Manager & si3m, uint uID)
	{
		construct();
		load(si3m, uID);
	}
	unique_sound::unique_sound(const sound_data & soundd)
	{
		construct();
		set_sound_data(soundd);
	}

	bool unique_sound::load(Manager & si3m, uint uID)
	{
		sound_data * pSoundData = new sound_data();
		bool bResult = pSoundData->load(si3m, uID);
		set_sound_data(*pSoundData);
		ownership = true;

		return bResult;
	}
	bool unique_sound::set_sound_data(const sound_data & soundd)
	{
		bool bResult = true;
		release();

		pSoundData = &soundd;
		ownership = false;


		bResult = pSoundData->create_buffer(secondarysb);
		secondarysb->GetFrequency(&BaseFreq);

		return bResult;
	}

	void unique_sound::replay()	// �Đ����J�n����B���ɍĐ����Ȃ�ŏ�����Đ�������
	{
		replay(false);
	}
	void unique_sound::reloop()	// ���[�v�Đ����J�n����B���ɍĐ����Ȃ�ŏ�����Đ�������
	{
		replay(true);
	}
	void unique_sound::stop()
	{
		secondarysb->Stop();
		secondarysb->SetCurrentPosition(0);
	}
	void unique_sound::replay(bool bLoopFlag)
	{
		DWORD dwFlag = NULL;
		if (bLoopFlag) dwFlag = DSBPLAY_LOOPING;

		stop();
		HRESULT result = secondarysb->Play(NULL, 0, dwFlag);

		if (result == DSERR_BUFFERLOST)	// �o�b�t�@���X�g�̏ꍇ�̓Z�J���_���o�b�t�@���쐬������
		{
			secondarysb->Restore();
			pSoundData->create_buffer(secondarysb);
			secondarysb->GetFrequency(&BaseFreq);
		}
	}


	void unique_sound::volume(float percent)
	{
		HRESULT result = secondarysb->SetVolume(static_cast<long>(10000 * percent));
		if (SUCCEEDED(result))
		{
			result += 0;
		}
		else
		{
			result -= 0;
		}
	}
	float unique_sound::volume() const
	{
		long val;
		secondarysb->GetVolume(&val);
		return val / 10000.0f;
	}


	void unique_sound::release()
	{
		if (ownership)
		{
			if(pSoundData)
			{
				delete pSoundData;
			}
		}

		pSoundData = 0;	// NULL

		dxsaferelease(secondarysb);
	}

	unique_sound::~unique_sound()
	{
		release();
	}

}
#include "unique_sound.h"
#include "Manager.h"
#include "saferelease.h"

namespace si3
{
	void UniqueSound::construct()
	{
		soundd = 0;	// NULL
		ownership = false;
		secondarysb = nullptr;
	}

	UniqueSound::UniqueSound()
	{
		construct();
	}
	UniqueSound::UniqueSound(Manager & si3m, uint id)
	{
		construct();
		load(si3m, id);
	}
	UniqueSound::UniqueSound(const SoundData & soundd)
	{
		construct();
		set_sound_data(soundd);
	}

	bool UniqueSound::load(Manager & si3m, uint id)
	{
		SoundData * soundd = new SoundData();
		bool bResult = soundd->load(si3m, id);
		set_sound_data(*soundd);
		ownership = true;

		return bResult;
	}
	bool UniqueSound::set_sound_data(const SoundData & soundd)
	{
		bool result = true;
		release();

		this->soundd = &soundd;
		ownership = false;


		result = soundd.create_buffer(secondarysb);
		secondarysb->GetFrequency(&base_freq);

		return result;
	}

	void UniqueSound::replay()	// 再生を開始する。既に再生中なら最初から再生し直す
	{
		replay(false);
	}
	void UniqueSound::reloop()	// ループ再生を開始する。既に再生中なら最初から再生し直す
	{
		replay(true);
	}
	void UniqueSound::stop()
	{
		secondarysb->Stop();
		secondarysb->SetCurrentPosition(0);
	}
	void UniqueSound::replay(bool bLoopFlag)
	{
		DWORD dwFlag = NULL;
		if (bLoopFlag) dwFlag = DSBPLAY_LOOPING;

		stop();
		HRESULT result = secondarysb->Play(NULL, 0, dwFlag);

		if (result == DSERR_BUFFERLOST)	// バッファロストの場合はセカンダリバッファを作成し直す
		{
			secondarysb->Restore();
			soundd->create_buffer(secondarysb);
			secondarysb->GetFrequency(&base_freq);
		}
	}


	void UniqueSound::volume(float percent)
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
	float UniqueSound::volume() const
	{
		long val;
		secondarysb->GetVolume(&val);
		return val / 10000.0f;
	}


	void UniqueSound::release()
	{
		if (ownership)
		{
			if(soundd)
			{
				delete soundd;
			}
		}

		soundd = 0;	// NULL

		dxsaferelease(secondarysb);
	}

	UniqueSound::~UniqueSound()
	{
		release();
	}

}
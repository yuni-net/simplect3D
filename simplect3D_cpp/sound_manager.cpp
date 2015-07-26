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
		window_.invisible().create();	// 透明ウィンドウにしておくこと

		HRESULT hr;

		// IDirectSound8インターフェイスの取得
		hr = DirectSoundCreate8(NULL, &dxs, NULL);
		if (FAILED(hr))
		{
			fw::popup("IDirectSound8インターフェイスの取得に失敗");
			return false;
		}

		//ここで協調レベルを設定
		hr = dxs->SetCooperativeLevel(window_.myhandle(), DSSCL_PRIORITY);
		if (FAILED(hr))
		{
			// 協調レベルの設定に失敗
			fw::popup("協調レベルの設定に失敗");
			return false;
		}

		// プライマリ・バッファの作成
		// DSBUFFERDESC構造体を設定
		DSBUFFERDESC dsbdesc;
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		// プライマリ・バッファを指定
		dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwBufferBytes = 0;
		dsbdesc.lpwfxFormat = NULL;

		// バッファを作る
		LPDIRECTSOUNDBUFFER primarysb;
		hr = dxs->CreateSoundBuffer(&dsbdesc, &primarysb, NULL);
		if (FAILED(hr))
		{
			// バッファの作成に失敗
			fw::popup("プライマリ・バッファの作成に失敗");
			return false;
		}

		// プライマリ・バッファのWaveフォーマットを設定
		// 　　　優先協調レベル以上の協調レベルが設定されている必要があります．
		WAVEFORMATEX pcmwf;
		ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX));
		pcmwf.wFormatTag = WAVE_FORMAT_PCM;
		pcmwf.nChannels = 2;			// ２チャンネル（ステレオ）
		pcmwf.nSamplesPerSec = 44100;	// サンプリング・レート　44.1kHz
		pcmwf.nBlockAlign = 4;
		pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
		pcmwf.wBitsPerSample = 16;		// 16ビット
		hr = primarysb->SetFormat(&pcmwf);
		if (FAILED(hr))
		{
			fw::popup("プライマリ・バッファのフォーマット設定に失敗");
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
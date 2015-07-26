#include "si3_SoundData.h"
#include "si3_SoundManager.h"
#include "si3_Manager.h"
#include "si3_saferelease.h"

namespace si3
{

	bool SoundData::load(Manager & si3m, uint id)
	{
		this->id = id;
		si3mp = &si3m;

		SoundManager & soundm = si3m.get_sound_manager();
		const char * path = soundm.get_path(id);

		// WAVEファイルを開く
		if (wav_data.load(path) == false)
		{
			fw::popup("failed to load Wave File");
			return false;
		}

		// セカンダリ・バッファを作成する
		// WAVEFORMATEX構造体を設定
		wfe.wFormatTag = wav_data.format_tag();
		wfe.nChannels = wav_data.channels();
		wfe.nSamplesPerSec = wav_data.samples_per_sec();
		wfe.nAvgBytesPerSec = wav_data.avg_bytes_per_sec();
		wfe.nBlockAlign = wav_data.block_align();
		wfe.wBitsPerSample = wav_data.bits_per_sample();
		wfe.cbSize = wav_data.pwf_size();

		// DSBUFFERDESC構造体を設定
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
			DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
		dsbdesc.dwBufferBytes = wav_data.size();
		dsbdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
		dsbdesc.lpwfxFormat = &wfe;



		return true;
	}

	uint SoundData::get_ID() const
	{
		return id;
	}


	bool SoundData::create_buffer(IDirectSoundBuffer8 * & secondarysb) const
	{
		HRESULT hr;

		SoundManager & soundm = si3mp->get_sound_manager();

		// バッファを作る
		LPDIRECTSOUNDBUFFER dsbuffer;
		hr = soundm.get_interface()->CreateSoundBuffer(&dsbdesc, &dsbuffer, NULL);
		if (FAILED(hr))
		{
			fw::popup("failed to secondary buffer");
			return false;
		}
		hr = dsbuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<LPVOID *>(&secondarysb));
		dxsaferelease(dsbuffer);
		if (FAILED(hr))
		{
			fw::popup("IDirectSoundBuffer8インターフェイスの取得に失敗");
			return false;
		}

		// セカンダリ・バッファにWaveデータを書き込む
		LPVOID block_ptr1; 	// 最初のブロックのポインタ
		DWORD block_bytes1; 	// 最初のブロックのサイズ
		LPVOID block_ptr2; 	// ２番目のブロックのポインタ
		DWORD block_bytes2; 	// ２番目のブロックのサイズ

		hr = secondarysb->Lock(0, wav_data.size(), &block_ptr1, &block_bytes1, &block_ptr2, &block_bytes2, 0);

		// DSERR_BUFFERLOSTが返された場合，Restoreメソッドを使ってバッファを復元する
		if (DSERR_BUFFERLOST == hr)
		{
			secondarysb->Restore();
			hr = secondarysb->Lock(0, wav_data.size(), &block_ptr1, &block_bytes1, &block_ptr2, &block_bytes2, 0);
		}
		if (FAILED(hr))
		{
			fw::popup("failed to lock secondary buffer");
			return false;
		}

		// ロック成功

		// ここで，バッファに書き込む
		// バッファにデータをコピーする
		memcpy(block_ptr1, wav_data.data(), block_bytes1);
		if (0 != block_bytes2) memcpy(block_ptr2, fw::pointer_cast<const char *>(wav_data.data()) + block_bytes1, block_bytes2);

		// 書き込みが終わったらすぐにUnlockする．
		hr = secondarysb->Unlock(block_ptr1, block_bytes1, block_ptr2, block_bytes2);
		if (FAILED(hr)) return false;

		return true;
	}




	SoundData::SoundData()
	{
		construct();
	}
	SoundData::SoundData(Manager & si3m, uint id)
	{
		construct();
		load(si3m, id);
	}


	void SoundData::construct()
	{
		// Nothing
	}

}
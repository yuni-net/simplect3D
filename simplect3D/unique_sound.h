#ifndef biringal3D_unique_sound_h_
#define biringal3D_unique_sound_h_

#include "popular.h"
#include "sound_data.h"

namespace si3
{
	class UniqueSound
	{
	public:
		UniqueSound();
		UniqueSound(Manager & si3m, uint id);
		UniqueSound(const SoundData & soundd);

		bool load(Manager & si3m, uint id);
		bool set_sound_data(const SoundData & soundd);

		void replay();	// 再生を開始する。既に再生中なら最初から再生し直す
		void reloop();	// ループ再生を開始する。既に再生中なら最初から再生し直す
	//	void play();	// 再生されている状態にする。つまり既に再生中なら何もしない
	//	void loop();	// ループ再生されている状態にする。つまり既に再生中なら何もしない
		void stop();
		void replay(bool do_loop);
		void volume(float percent);
		float volume() const;



		~UniqueSound();
	private:
		const SoundData * soundd;
		bool ownership;

		IDirectSoundBuffer8 * secondarysb;
		DWORD base_freq;

		void construct();
		void release();
	};
}

#endif
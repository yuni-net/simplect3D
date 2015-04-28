#ifndef biringal3D_unique_sound_h_
#define biringal3D_unique_sound_h_

#include "popular.h"
#include "sound_data.h"

namespace si3
{
	class unique_sound
	{
	public:
		unique_sound();
		unique_sound(manager & si3m, uint uID);
		unique_sound(const sound_data & soundd);

		bool load(manager & si3m, uint uID);
		bool set_sound_data(const sound_data & soundd);

		void replay();	// 再生を開始する。既に再生中なら最初から再生し直す
		void reloop();	// ループ再生を開始する。既に再生中なら最初から再生し直す
	//	void play();	// 再生されている状態にする。つまり既に再生中なら何もしない
	//	void loop();	// ループ再生されている状態にする。つまり既に再生中なら何もしない
		void stop();
		void replay(bool bLoopFlag);
		void volume(float percent);
		float volume() const;



		~unique_sound();
	private:
		const sound_data * pSoundData;
		bool ownership;

		IDirectSoundBuffer8 * secondarysb;
		DWORD BaseFreq;

		void construct();
		void release();
	};
}

#endif
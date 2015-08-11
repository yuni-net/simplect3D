#ifndef biringal3D_unique_sound_h_
#define biringal3D_unique_sound_h_

#include "si3_popular.h"
#include "si3_SoundData.h"

namespace si3
{
	class UniqueSound
	{
	public:
		UniqueSound();
		UniqueSound(uint id);
		UniqueSound(const SoundData & soundd);

		bool load(uint id);
		bool set_sound_data(const SoundData & soundd);

		void replay();	// �Đ����J�n����B���ɍĐ����Ȃ�ŏ�����Đ�������
		void reloop();	// ���[�v�Đ����J�n����B���ɍĐ����Ȃ�ŏ�����Đ�������
	//	void play();	// �Đ�����Ă����Ԃɂ���B�܂���ɍĐ����Ȃ牽�����Ȃ�
	//	void loop();	// ���[�v�Đ�����Ă����Ԃɂ���B�܂���ɍĐ����Ȃ牽�����Ȃ�
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
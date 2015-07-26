#ifndef biringal3D_se_h_
#define biringal3D_se_h_

#include "si3_popular.h"
#include "si3_UniqueSound.h"
#include "si3_SoundData.h"

namespace si3
{
	class SE
	{
	public:
		// max_moment�ŁA�����ɍĐ��ł���SE�̍ő吔��ݒ肷��B
		// ���̐��l���傫���قǃ������͑���������B
		// �ȗ������ꍇ�̓f�t�H���g�l(16)���g�p�����B
		SE();
		SE(Manager & si3m, uint uID);
		SE(Manager & si3m, uint uID, int max_moment);
		SE(const SoundData & soundd);
		SE(const SoundData & soundd, int max_moment);

		bool load(Manager & si3m, uint uID);
		bool load(Manager & si3m, uint uID, int max_moment);
		bool set_sound_data(const SoundData & soundd);
		bool set_sound_data(const SoundData & soundd, int max_moment);

		// �V�����Đ����J�n����B
		// ���ɍĐ�����Ă�����̂ɉe����^���邱�Ƃ͂Ȃ��A�t�ɉe�����󂯂邱�Ƃ��Ȃ��B
		// �����������ɍĐ��ł���SE�̍ő吔�𒴂����ꍇ�A�ł��Â����̂̍Đ����~������A�V�����Đ����J�n����B
		void play();



		~SE();
	private:
		fw::Array<UniqueSound> se_list;
		int next_index;
		const SoundData * soundd;
		bool ownership;

		void construct();
		void release();
	};
}

#endif
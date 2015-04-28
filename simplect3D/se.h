#ifndef biringal3D_se_h_
#define biringal3D_se_h_

#include "popular.h"
#include "unique_sound.h"
#include "sound_data.h"

namespace si3
{
	class se
	{
	public:
		// nMaxMoment�ŁA�����ɍĐ��ł���se�̍ő吔��ݒ肷��B
		// ���̐��l���傫���قǃ������͑���������B
		// �ȗ������ꍇ�̓f�t�H���g�l(16)���g�p�����B
		se();
		se(manager & si3m, uint uID);
		se(manager & si3m, uint uID, int nMaxMoment);
		se(const sound_data & soundd);
		se(const sound_data & soundd, int nMaxMoment);

		bool load(manager & si3m, uint uID);
		bool load(manager & si3m, uint uID, int nMaxMoment);
		bool set_sound_data(const sound_data & soundd);
		bool set_sound_data(const sound_data & soundd, int nMaxMoment);

		// �V�����Đ����J�n����B
		// ���ɍĐ�����Ă�����̂ɉe����^���邱�Ƃ͂Ȃ��A�t�ɉe�����󂯂邱�Ƃ��Ȃ��B
		// �����������ɍĐ��ł���se�̍ő吔�𒴂����ꍇ�A�ł��Â����̂̍Đ����~������A�V�����Đ����J�n����B
		void play();



		~se();
	private:
		fw::vector<unique_sound> se_list;
		int nNextIndex;
		const sound_data * pSoundData;
		bool bOwnerShip;

		void construct();
		void release();
	};
}

#endif
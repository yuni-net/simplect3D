#include "si3_SE.h"

namespace si3
{
	// max_moment�ŁA�����ɍĐ��ł���SE�̍ő吔��ݒ肷��B
	// ���̐��l���傫���قǃ������͑���������B
	// �ȗ������ꍇ�̓f�t�H���g�l(16)���g�p�����B
	SE::SE()
	{
		construct();
	}
	SE::SE(Manager & si3m, uint uID)
	{
		construct();
		load(si3m, uID);
	}
	SE::SE(Manager & si3m, uint uID, int max_moment)
	{
		construct();
		load(si3m, uID, max_moment);
	}
	SE::SE(const SoundData & soundd)
	{
		construct();
		set_sound_data(soundd);
	}
	SE::SE(const SoundData & soundd, int max_moment)
	{
		construct();
		set_sound_data(soundd, max_moment);
	}

	bool SE::load(Manager & si3m, uint uID)
	{
		return load(si3m, uID, 16);
	}
	bool SE::load(Manager & si3m, uint uID, int max_moment)
	{
		bool bResult = true;

		SoundData * soundd = new SoundData();
		bResult = soundd->load(si3m, uID);
		if (bResult == false) return false;

		bResult = set_sound_data(*soundd, max_moment);

		ownership = true;

		return bResult;
	}
	bool SE::set_sound_data(const SoundData & soundd)
	{
		return set_sound_data(soundd, 16);
	}
	bool SE::set_sound_data(const SoundData & soundd, int max_moment)
	{
		bool bResult = true;


		release();
		this->soundd = &soundd;

		se_list.setsize(max_moment);

		for(uint uIndex=0;uIndex<se_list.size();++uIndex)
		{
			if (se_list[uIndex].set_sound_data(soundd) == false)
			{
				bResult = false;
			}
		}

		return bResult;
	}

	// �V�����Đ����J�n����B
	// ���ɍĐ�����Ă�����̂ɉe����^���邱�Ƃ͂Ȃ��A�t�ɉe�����󂯂邱�Ƃ��Ȃ��B
	// �����������ɍĐ��ł���SE�̍ő吔�𒴂����ꍇ�A�ł��Â����̂̍Đ����~������A�V�����Đ����J�n����B
	void SE::play()
	{
		se_list[next_index].replay();
		++next_index;
		next_index %= se_list.size();
	}





	void SE::construct()
	{
		next_index = 0;
		soundd = nullptr;
		ownership = false;
	}

	void SE::release()
	{
		if (ownership)
		{
			if (soundd)
			{
				delete soundd;
			}
		}

		soundd = nullptr;
		ownership = false;
	}

	SE::~SE()
	{
		release();
	}

}
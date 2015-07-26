#include "si3_SE.h"

namespace si3
{
	// max_momentで、同時に再生できるSEの最大数を設定する。
	// この数値が大きいほどメモリは多く消費される。
	// 省略した場合はデフォルト値(16)が使用される。
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

	// 新しく再生を開始する。
	// 既に再生されているものに影響を与えることはなく、逆に影響を受けることもない。
	// ただし同時に再生できるSEの最大数を超えた場合、最も古いものの再生を停止した後、新しく再生を開始する。
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
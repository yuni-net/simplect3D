#include "se.h"

namespace si3
{
	// nMaxMomentで、同時に再生できるseの最大数を設定する。
	// この数値が大きいほどメモリは多く消費される。
	// 省略した場合はデフォルト値(16)が使用される。
	se::se()
	{
		construct();
	}
	se::se(Manager & si3m, uint uID)
	{
		construct();
		load(si3m, uID);
	}
	se::se(Manager & si3m, uint uID, int nMaxMoment)
	{
		construct();
		load(si3m, uID, nMaxMoment);
	}
	se::se(const sound_data & soundd)
	{
		construct();
		set_sound_data(soundd);
	}
	se::se(const sound_data & soundd, int nMaxMoment)
	{
		construct();
		set_sound_data(soundd, nMaxMoment);
	}

	bool se::load(Manager & si3m, uint uID)
	{
		return load(si3m, uID, 16);
	}
	bool se::load(Manager & si3m, uint uID, int nMaxMoment)
	{
		bool bResult = true;

		sound_data * pSoundData = new sound_data();
		bResult = pSoundData->load(si3m, uID);
		if (bResult == false) return false;

		bResult = set_sound_data(*pSoundData, nMaxMoment);

		bOwnerShip = true;

		return bResult;
	}
	bool se::set_sound_data(const sound_data & soundd)
	{
		return set_sound_data(soundd, 16);
	}
	bool se::set_sound_data(const sound_data & soundd, int nMaxMoment)
	{
		bool bResult = true;


		release();
		pSoundData = &soundd;

		se_list.setsize(nMaxMoment);

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
	// ただし同時に再生できるseの最大数を超えた場合、最も古いものの再生を停止した後、新しく再生を開始する。
	void se::play()
	{
		se_list[nNextIndex].replay();
		++nNextIndex;
		nNextIndex %= se_list.size();
	}





	void se::construct()
	{
		nNextIndex = 0;
		pSoundData = nullptr;
		bOwnerShip = false;
	}

	void se::release()
	{
		if (bOwnerShip)
		{
			if (pSoundData)
			{
				delete pSoundData;
			}
		}

		pSoundData = nullptr;
		bOwnerShip = false;
	}

	se::~se()
	{
		release();
	}

}
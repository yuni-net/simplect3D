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
		// nMaxMomentで、同時に再生できるseの最大数を設定する。
		// この数値が大きいほどメモリは多く消費される。
		// 省略した場合はデフォルト値(16)が使用される。
		se();
		se(manager & si3m, uint uID);
		se(manager & si3m, uint uID, int nMaxMoment);
		se(const sound_data & soundd);
		se(const sound_data & soundd, int nMaxMoment);

		bool load(manager & si3m, uint uID);
		bool load(manager & si3m, uint uID, int nMaxMoment);
		bool set_sound_data(const sound_data & soundd);
		bool set_sound_data(const sound_data & soundd, int nMaxMoment);

		// 新しく再生を開始する。
		// 既に再生されているものに影響を与えることはなく、逆に影響を受けることもない。
		// ただし同時に再生できるseの最大数を超えた場合、最も古いものの再生を停止した後、新しく再生を開始する。
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
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
		// max_momentで、同時に再生できるSEの最大数を設定する。
		// この数値が大きいほどメモリは多く消費される。
		// 省略した場合はデフォルト値(16)が使用される。
		SE();
		SE(Manager & si3m, uint uID);
		SE(Manager & si3m, uint uID, int max_moment);
		SE(const SoundData & soundd);
		SE(const SoundData & soundd, int max_moment);

		bool load(Manager & si3m, uint uID);
		bool load(Manager & si3m, uint uID, int max_moment);
		bool set_sound_data(const SoundData & soundd);
		bool set_sound_data(const SoundData & soundd, int max_moment);

		// 新しく再生を開始する。
		// 既に再生されているものに影響を与えることはなく、逆に影響を受けることもない。
		// ただし同時に再生できるSEの最大数を超えた場合、最も古いものの再生を停止した後、新しく再生を開始する。
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
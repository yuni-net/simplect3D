#pragma once

#include "si3_popular.h"

/*
 * 描画のfpsを60にする目的で使用される
 * 全ての描画を終えた後、前回の描画完了時から33.3ms以上経過していた場合のみ処理を先に進める用途を想定している
 * 前回stop_if_needが呼ばれてから33.3ms以上経過していればstop_if_need関数から脱出する
 *
 */
namespace si3
{
	class Fps60
	{
	public:
		void init();
		void stop_if_need();


		~Fps60();
	private:
		DWORD old_ms;
	};
}
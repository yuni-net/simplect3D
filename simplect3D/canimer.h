#ifndef canimer_h_
#define canimer_h_

#include "fw_includes.h"

namespace si3
{
	class canimer
	{
	public:
		canimer();
		void config(int frame/*アニメコマ数*/, int resistor/*アニメーションスピード抑制値。値が大きいほどアニメーションが遅くなる*/);
		void frame(int v);
		void resistor(int v);
		void set(int No);		// アニメーション位置を強制的に設定します。0～maxNo()までの整数で指定します
		void move(int num);		// アニメーション位置を指定数値分進ませます。マイナスの値も指定可能です
		int now() const;		// 現在何コマ目なのかを返します ※アニメーション位置ではありません！
		int nowNo() const;
		int maxNo() const;		// アニメーション位置の最大値を返します

	private:
		int counter;
		int nFrame;
		int nResistor;
	};
}

#endif
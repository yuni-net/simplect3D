#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"

namespace si3
{
	class manager;
	class ModelData;

	class MotionData
	{
	public:
		/***
		** モーションデータのロードを行います。
		** この時点でアニメーション位置が先頭に設定されます。
		** また、アニメーションを再生状態にします。
		*/
		void load(manager & si3m, const char * path);

		/***
		** アニメーションを再生状態にします
		*/
		void play();

		/***
		** アニメーションを一時停止状態にします
		*/
		void pause();

		/***
		** アニメーション位置を先頭に設定します
		*/
		void seek_first();




		/***
		** この関数はユーザーが呼び出すのではなく、Motion3Dクラス側が呼び出しを行う関数です。
		** プレイ状態のときにアニメーションを行います。
		** アニメーションによって変更された新しい頂点情報をmodel_dataに転送します。
		*/
		void animation(ModelData & model_data);

		/***
		** ボーンと頂点の関係データを白紙に戻します。
		*/
		void clear_top_lists();

		/***
		** ボーンと頂点の関係データに新しい関係を追加します。
		*/
		void add_top(Top_pmd & top);

	private:
		struct BonedTop
		{
			float pos[3];
			float normal[3];
			unsigned char bone_weight;

			void init(const Top_pmd & top, unsigned char bone_weight)
			{
				pos[0] = top.pos[0];
				pos[1] = top.pos[1];
				pos[2] = top.pos[2];

				normal[0] = top.normal[0];
				normal[1] = top.normal[1];
				normal[2] = top.normal[2];

				bone_weight = top.bone_weight;
			}
		};

		bool animating;
		int now_frame;
		bool did_seek_first;
		fw::vector<fw::vector<BonedTop> > top_lists;
	};
}

#endif
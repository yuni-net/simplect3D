#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"
#include "matrix.h"
#include "si3_coor3.h"
#include "BoneMap.h"
#include "BoneData.h"

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
		void load(manager & si3m, const char * path, const BoneMap & bone_map);

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
		** アニメーションが終了していればtrueを、
		** 終了していなければfalseを返します。
		*/
		bool did_finish_anime() const;




		/***
		** この関数はユーザーが呼び出すのではなく、Motion3Dクラス側が呼び出しを行う関数です。
		** プレイ状態のときにアニメーションを行います。
		** アニメーションによって変更された新しい頂点情報をmodel_dataに転送します。
		*/
		void animation(ModelData & model_data);

		/***
		** ボーンと頂点の関係データを白紙に戻します。
		*/
		void init_top_lists(const int bone_num);

		/***
		** ボーンと頂点の関係データに新しい関係を追加します。
		*/
		void add_top(int index, Top_pmd & top);

	private:


		bool animating;
		int now_frame;
		int final_frame;
		bool did_seek_first;
		fw::vector<BoneData> bone_lists;

		int bone_num() const;
	};
}

#endif
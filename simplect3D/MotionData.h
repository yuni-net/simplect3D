#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"
#include "matrix.h"
#include "si3_coor3.h"
#include "BoneMap.h"

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
		struct BonedTop
		{
			int index;
			float pos[3];
			float normal[3];
			float bone_weight;

			void init(int index, const Top_pmd & top, float bone_weight)
			{
				this->index = index;

				pos[0] = top.pos[0];
				pos[1] = top.pos[1];
				pos[2] = top.pos[2];

				normal[0] = top.normal[0];
				normal[1] = top.normal[1];
				normal[2] = top.normal[2];

				bone_weight = top.bone_weight;
			}
		};

		struct KeyFrame
		{
			int frame;
			coor3 pos;
			float radian;
			coor3 axis;
		};

		struct MoveData
		{
			const KeyFrame * beg;
			const KeyFrame * end;
		};

		struct BoneData
		{
			fw::vector<BonedTop> associated_tops;
			fw::vector<KeyFrame> key_frame_list;
		};

		bool animating;
		int now_frame;
		int final_frame;
		bool did_seek_first;
		fw::vector<BoneData> bone_lists;

		int bone_num() const;
		bool did_bone_move(MoveData & move_data, int bone_No) const;
		matrix rot_mat_of_bone(const MoveData & move_data) const;
		matrix para_mat_of_bone(const MoveData & move_data) const;
		int top_num_of_bone(int bone_No) const;
		const BonedTop & top_of_bone(int bone_No, int top_No);
		void trans_and_set(D3DVECTOR & target, const float base[3], const matrix & mat);
	};
}

#endif
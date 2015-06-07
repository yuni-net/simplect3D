#ifndef si3_BoneMotion_h_
#define si3_BoneMotion_h_

#include "popular.h"
#include "si3_coor3.h"
#include "Top_pmd.h"
#include "util_MMD.h"
#include "matrix.h"

namespace si3
{
	class BoneMotion
	{
	public:

		/***
		* @brief キーフレーム情報を追加します。
		*/
		void add_key_frame(const Motion & motion);


		/***
		* @brief 現在のフレームのこのボーンの座標変換行列を計算します。
		* @param
		*  [out]trans_mat: 変換行列がここに格納されます
		*  [out]rot_mat: 変換行列の回転成分のみがここに格納されます
		*  now_frame: 現在のフレーム指定します
		* @return
		*  座標を変換する必要がある場合はtrueを、そもそも変換する必要が無い場合はfalseを返します。
		*  例えば前のフレームから変化が無ければ座標を再度変換する必要は無いわけです。
		*  ただし、必ず１フレームずつ順番にアニメーションすることが前提になっています。
		*  逆再生や、任意のフレームにテレポートした場合は当然変換する必要があるでしょう。
		*/
		bool compute_trans_mat(
			matrix & trans_mat,
			matrix & rot_mat,
			const int now_frame);



	private:
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


		
		fw::vector<KeyFrame> key_frame_list;


		void compute_move_data(
			MoveData & move_data,
			bool & todo_trans,
			bool & todo_judge_move,
			const int now_frame);

		/***
		* move_dataのbegとendが異なる値であるかどうかを判定します。
		* 異なる場合はtrueを、同じである場合はfalseを返します。
		*/
		bool did_bone_move(MoveData & move_data) const;

		matrix rot_mat_of_bone(const MoveData & move_data) const;
		matrix para_mat_of_bone(const MoveData & move_data) const;

	};
}

#endif
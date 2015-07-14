#ifndef si3_BoneMotion_h_
#define si3_BoneMotion_h_

#include <list>
#include "popular.h"
#include "si3_coor3.h"
#include "Top_pmd.h"
#include "util_MMD.h"
#include "matrix.h"
#include "utility.h"

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
		* 全てのキーフレーム情報の追加が終了したら必ず呼び出してください。
		*/
		void finish_add_key_frame();


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
			const Coor3 & bone_pos,
			const int now_frame);



	private:
		struct KeyFrame
		{
			int frame;
			Coor3 pos;
			Quaternion quate;

			bool operator<(const KeyFrame & another) const
			{
				return frame < another.frame;
			}
		};

		struct MoveData
		{
			const KeyFrame * beg;
			const KeyFrame * end;
		};


		
		std::vector<KeyFrame> key_frame_list;


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

		float compute_percent(MoveData & move_data, const int now_frame) const;

		/***
		* @brief move_dataに基づき現在のフレームの回転行列を計算してそれを返す。
		* @param
		*  move_data: MoveDataのインスタンスを指定します。
		*  percent: begフレームとendフレームの間のどこに居るのかを割合で指定します(0.0f-1.0f)。
		*  [out]quate: 計算の結果得られたクォータニオンがここに格納されます。
		* @return 現在のフレームの回転行列を返す。
		*/
		matrix rot_mat_of_bone(
			const MoveData & move_data,
			const float percent,
			D3DXQUATERNION & quate) const;

	//	matrix para_mat_of_bone(const MoveData & move_data, const float percent) const;

		/***
		* @brief move_dataに基づき現在のフレームの変換行列を計算してそれを返す。
		* @param
		*  move_data: MoveDataのインスタンスを指定します。
		*  percent: begフレームとendフレームの間のどこに居るのかを割合で指定します(0.0f-1.0f)。
		*  bone_top: ボーンの頂点を指定します。これが回転の中心座標となります。
		*  quate: 回転に使用するクォータニオンをここに指定します。
		* @return 現在のフレームの変換行列を返す。
		*/
		matrix trans_mat_of_bone(
			const MoveData & move_data,
			const float percent,
			const Coor3 & bone_top,
			const D3DXQUATERNION & quate) const;


		bool axis_is_unit(const Coor3 & axis) const;
	};
}

#endif
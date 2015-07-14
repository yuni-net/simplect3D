#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"
#include "matrix.h"
#include "si3_coor3.h"
#include "BoneMap.h"
#include "BoneMotion.h"

namespace si3
{
	class Manager;
	class ModelData;

	class MotionData
	{
	public:
		/***
		** モーションデータのロードを行います。
		** この時点でアニメーション位置が先頭に設定されます。
		** また、アニメーションを再生状態にします。
		*/
		void load(Manager & si3m, const char * path, const BoneMap & bone_map, const int bone_num);

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
		** この関数はユーザーが呼び出すのではなく、ModelDataクラス側が呼び出しを行う関数です。
		** プレイ状態のときにアニメーションカウンタを進めます。
		** アニメーションを行う必要が無い場合はtrueを返します。
		*/
		bool animation();

		/***
		* @brief 現在のフレームのこのボーンの座標変換行列を計算します。
		* @param
		*  [out]trans_mat: 変換行列がここに格納されます
		*  [out]rot_mat: 変換行列の回転成分のみがここに格納されます
		*  bone_No: ボーン番号を指定します
		* @return
		*  座標を変換する必要がある場合はtrueを、そもそも変換する必要が無い場合はfalseを返します。
		*  例えば前のフレームから変化が無ければ座標を再度変換する必要は無いわけです。
		*  ただし、必ず１フレームずつ順番にアニメーションすることが前提になっています。
		*  逆再生や、任意のフレームにテレポートした場合は当然変換する必要があるでしょう。
		*/
		bool compute_trans_mat(
			matrix & trans_mat,
			matrix & rot_mat,
			const coor3 & bone_pos,
			const int bone_No);



	private:


		bool animating;
		int now_frame;
		int final_frame;
		bool did_seek_first;
		fw::Array<BoneMotion> motion_list;
	};
}

#endif
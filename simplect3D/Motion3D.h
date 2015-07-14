#ifndef Motion3D_h_
#define Motion3D_h_

#include "si3_coor3.h"
#include "display_object.h"
#include "Matrix.h"
#include "MotionData.h"
#include "ModelData_pmd.h"

namespace si3
{
	class Manager;

	class Motion3D :public DisplayObject
	{
	public:
		Coor3 pos;
		Coor3 radian;

		/***
		** モデルデータとモーションデータを読み込みます。
		** モデルデータはpmd形式、モーションデータはvmd形式のみ対応です。
		** この時点でアニメーション位置が先頭に設定されます。
		** また、回転角はxyz全てゼロに設定されます。
		*/
		void load(Manager & si3m, const char * model_path, const char * motion_path);

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






		// @override
		/***
		** この関数はユーザー側が呼び出すのではなく、simplect3Dライブラリのシステムが呼び出します。
		** この時点でアニメーションが行われます。
		*/
		void draw_no_alpha() const;

		// @override
		/***
		** この関数はユーザー側が呼び出すのではなく、simplect3Dライブラリのシステムが呼び出します。
		*/
		void draw_alpha() const;



	private:
		mutable ModelData_pmd model_data;
		mutable MotionData motion_data;
		BoneMap bone_map;
		mutable Matrix world_mat;
		LPDIRECT3DDEVICE9 device;

		void compute_world_mat() const;
	};
}

#endif
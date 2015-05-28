#include <MotionData.h>
#include <ModelData.h>

namespace si3
{
	/***
	** モーションデータのロードを行います。
	** この時点でアニメーション位置が先頭に設定されます。
	*/
	void MotionData::load(manager & si3m, const char * path)
	{
		// todo
		animating = true;
		now_frame = 0;
		did_seek_first = false;
	}

	/***
	** アニメーションを再生状態にします
	*/
	void MotionData::play()
	{
		animating = true;
	}

	/***
	** アニメーションを一時停止状態にします
	*/
	void MotionData::pause()
	{
		animating = false;
	}

	/***
	** アニメーション位置を先頭に設定します
	*/
	void MotionData::seek_first()
	{
		now_frame = 0;
	}




	/***
	** この関数はユーザーが呼び出すのではなく、Motion3Dクラス側が呼び出しを行う関数です。
	** プレイ状態のときにアニメーションを行います。
	** アニメーションによって変更された新しい頂点情報をmodel_dataに転送します。
	*/
	void MotionData::animation(ModelData & model_data)
	{
		// todo
	}

	/***
	** ボーンと頂点の関係データを白紙に戻します。
	*/
	void MotionData::clear_top_lists()
	{
		top_lists.zerosize();
	}

	/***
	** ボーンと頂点の関係データに新しい関係を追加します。
	*/
	void MotionData::add_top(Top_pmd & top)
	{
		int bonedex;
		BonedTop boned_top;

		bonedex = top.bone_num[0];
		boned_top.init(top, top.bone_weight);
		top_lists[bonedex].add(boned_top);

		// todo add bone2
	}

}
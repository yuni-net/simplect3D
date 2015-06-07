#include <Motion3D.h>
#include <manager.h>

namespace si3
{
	/***
	** モデルデータとモーションデータを読み込みます。
	** モデルデータはpmd形式、モーションデータはvmd形式のみ対応です。
	** この時点でアニメーション位置が先頭に設定されます。
	*/
	void Motion3D::load(manager & si3m, const char * model_path, const char * motion_path)
	{
		device = si3m.get_dxdevice();
		model_data.load(si3m, model_path, motion_data, bone_map);
		motion_data.load(si3m, motion_path, bone_map);
		seek_first();
		radian.x = 0.0f;
		radian.y = 0.0f;
		radian.z = 0.0f;
	}

	/***
	** アニメーションを再生状態にします
	*/
	void Motion3D::play()
	{
		motion_data.play();
	}

	/***
	** アニメーションを一時停止状態にします
	*/
	void Motion3D::pause()
	{
		motion_data.pause();
	}

	/***
	** アニメーション位置を先頭に設定します
	*/
	void Motion3D::seek_first()
	{
		motion_data.seek_first();
	}

	// @override
	void Motion3D::draw_no_alpha() const
	{
		model_data.animation(motion_data);

		compute_world_mat();

		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 半透明表示禁止(αブレンディング無効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		model_data.draw_no_alpha();
	}

	// @override
	void Motion3D::draw_alpha() const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// αテスト有効
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// 半透明描画有効
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		model_data.draw_alpha();
	}





	void Motion3D::compute_world_mat() const
	{
		matrix para_mat;
		para_mat.parallel(pos.x, pos.y, pos.z);

		matrix rot_mat =
			rot_mat.rotate_y(radian.y) *
			rot_mat.rotate_x(radian.x) *
			rot_mat.rotate_z(radian.z);

		world_mat = rot_mat * para_mat;
	}
}
#include <camera.h>
#include <matrix.h>


namespace si3
{
	bool camera::init(LPDIRECT3DDEVICE9 dxdevice, unsigned int screen_width, unsigned int screen_height)
	{
		const float view_angle = 60.0f;													// 視野角
		const float aspect_ratio = static_cast<float>(screen_width) / screen_height;	// アスペクト比
		near_clip_ = 0.1f;													// 前面クリップ距離
		const float far_clip = 10000.0f;												// 背面クリップ距離

		this->dxdevice = dxdevice;

		// プロジェクション マトリックス設定
		D3DXMatrixPerspectiveFovLH(&projection_mat, D3DXToRadian(view_angle), aspect_ratio, near_clip(), far_clip);

		// ビュー マトリックス設定
		camera_coor = D3DXVECTOR3(0.0f, 20.0f, -50.0f);						// 視点座標
		look_coor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 注視点座標
		up_vector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);							// アップベクトル
		radian = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		update();

		return true;

	}

	void camera::update()
	{
		// プロジェクション マトリックス設定
		dxdevice->SetTransform(D3DTS_PROJECTION, &projection_mat);


		// ビュー マトリックス設定
		matrix rotated_matrix;
		rotated_matrix.x(camera_coor.x);
		rotated_matrix.y(camera_coor.y);
		rotated_matrix.z(camera_coor.z);

		rotated_matrix *= matrix().rotate_x(radian.x)*matrix().rotate_y(radian.y)*matrix().rotate_z(radian.z);

#if 0
		// ビュー マトリックス設定
		matrix rotated_matrix;
		rotated_matrix.x(camera_coor.x - look_coor.x);
		rotated_matrix.y(camera_coor.y - look_coor.y);
		rotated_matrix.z(camera_coor.z - look_coor.z);

		rotated_matrix *= matrix().rotate_x(radian.x)*matrix().rotate_y(radian.y)*matrix().rotate_z(radian.z);
		rotated_matrix *= matrix().trans(look_coor.x, look_coor.y, look_coor.z);
#endif


		D3DXVECTOR3 rotated_coor(rotated_matrix.x(), rotated_matrix.y(), rotated_matrix.z());

		// ビュー マトリックス
		D3DXMatrixLookAtLH(&view_mat_, &rotated_coor, &look_coor, &up_vector);
		dxdevice->SetTransform(D3DTS_VIEW, &view_mat_);

		// 回転成分だけ保存
		view_rot_ = view_mat_;

		view_rot_._41 = 0.0f;
		view_rot_._42 = 0.0f;
		view_rot_._43 = 0.0f;
		view_rot_._44 = 1.0f;

		view_rot_._14 = 0.0f;
		view_rot_._24 = 0.0f;
		view_rot_._34 = 0.0f;
	}


	void camera::x(float value)
	{
		camera_coor.x = value;
	}
	float camera::x() const
	{
		return camera_coor.x;
	}
	void camera::y(float value)
	{
		camera_coor.y = value;
	}
	float camera::y() const
	{
		return camera_coor.y;
	}
	void camera::z(float value)
	{
		camera_coor.z = value;
	}
	float camera::z() const
	{
		return camera_coor.z;
	}

	void camera::look_x(float value)
	{
		look_coor.x = value;
	}
	float camera::look_x() const
	{
		return look_coor.x;
	}
	void camera::look_y(float value)
	{
		look_coor.y = value;
	}
	float camera::look_y() const
	{
		return look_coor.y;
	}
	void camera::look_z(float value)
	{
		look_coor.z = value;
	}
	float camera::look_z() const
	{
		return look_coor.z;
	}

	void camera::up_x(float value)
	{
		up_vector.x = value;
	}
	float camera::up_x() const
	{
		return up_vector.x;
	}
	void camera::up_y(float value)
	{
		up_vector.y = value;
	}
	float camera::up_y() const
	{
		return up_vector.y;
	}
	void camera::up_z(float value)
	{
		up_vector.z = value;
	}
	float camera::up_z() const
	{
		return up_vector.z;
	}

	void camera::radian_x(float value)
	{
		radian.x = value;
	}
	float camera::radian_x() const
	{
		return radian.x;
	}
	void camera::radian_y(float value)
	{
		radian.y = value;
	}
	float camera::radian_y() const
	{
		return radian.y;
	}
	void camera::radian_z(float value)
	{
		radian.z = value;
	}
	float camera::radian_z() const
	{
		return radian.z;
	}

	const D3DXMATRIX & camera::get_projection_mat() const
	{
		return projection_mat;
	}

	const D3DXMATRIX & camera::view_mat() const
	{
		return view_mat_;
	}

	const D3DXMATRIX & camera::view_rot() const
	{
		return view_rot_;
	}

	float camera::near_clip() const
	{
		return near_clip_;
	}

}
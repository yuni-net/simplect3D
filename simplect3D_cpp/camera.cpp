#include <si3_Camera.h>
#include <si3_Matrix.h>


namespace si3
{
	bool Camera::init(
		LPDIRECT3DDEVICE9 dxdevice,
		unsigned int screen_width,
		unsigned int screen_height)
	{
		view_angle_ = 60.0f;													// 視野角
		near_clip_ = 10.0f;													// 前面クリップ距離
		far_clip_ = 10000.0f;												// 背面クリップ距離
		aspect_ratio = static_cast<float>(screen_width) / screen_height;	// アスペクト比

		this->dxdevice = dxdevice;

		// プロジェクション マトリックス設定
		update_proj_mat();

		// ビュー マトリックス設定
		camera_coor = D3DXVECTOR3(0.0f, 20.0f, -50.0f);						// 視点座標
		look_coor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 注視点座標
		up_vector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);							// アップベクトル
		radian = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		update();

		return true;

	}

	void Camera::update()
	{
		// プロジェクション マトリックス設定
		dxdevice->SetTransform(D3DTS_PROJECTION, &projection_mat);


		// ビュー マトリックス設定
		Matrix rotated_matrix;
		rotated_matrix.x(camera_coor.x);
		rotated_matrix.y(camera_coor.y);
		rotated_matrix.z(camera_coor.z);

		rotated_matrix *= Matrix().rotate_x(radian.x)*Matrix().rotate_y(radian.y)*Matrix().rotate_z(radian.z);


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


	void Camera::x(float value)
	{
		camera_coor.x = value;
	}
	float Camera::x() const
	{
		return camera_coor.x;
	}
	void Camera::y(float value)
	{
		camera_coor.y = value;
	}
	float Camera::y() const
	{
		return camera_coor.y;
	}
	void Camera::z(float value)
	{
		camera_coor.z = value;
	}
	float Camera::z() const
	{
		return camera_coor.z;
	}

	void Camera::look_x(float value)
	{
		look_coor.x = value;
	}
	float Camera::look_x() const
	{
		return look_coor.x;
	}
	void Camera::look_y(float value)
	{
		look_coor.y = value;
	}
	float Camera::look_y() const
	{
		return look_coor.y;
	}
	void Camera::look_z(float value)
	{
		look_coor.z = value;
	}
	float Camera::look_z() const
	{
		return look_coor.z;
	}

	void Camera::up_x(float value)
	{
		up_vector.x = value;
	}
	float Camera::up_x() const
	{
		return up_vector.x;
	}
	void Camera::up_y(float value)
	{
		up_vector.y = value;
	}
	float Camera::up_y() const
	{
		return up_vector.y;
	}
	void Camera::up_z(float value)
	{
		up_vector.z = value;
	}
	float Camera::up_z() const
	{
		return up_vector.z;
	}

	void Camera::radian_x(float value)
	{
		radian.x = value;
	}
	float Camera::radian_x() const
	{
		return radian.x;
	}
	void Camera::radian_y(float value)
	{
		radian.y = value;
	}
	float Camera::radian_y() const
	{
		return radian.y;
	}
	void Camera::radian_z(float value)
	{
		radian.z = value;
	}
	float Camera::radian_z() const
	{
		return radian.z;
	}

	const D3DXMATRIX & Camera::get_projection_mat() const
	{
		return projection_mat;
	}

	const D3DXMATRIX & Camera::view_mat() const
	{
		return view_mat_;
	}

	const D3DXMATRIX & Camera::view_rot() const
	{
		return view_rot_;
	}

	float Camera::near_clip() const
	{
		return near_clip_;
	}
	void Camera::near_clip(const float value)
	{
		near_clip_ = value;
		update_proj_mat();
	}
	float Camera::far_clip() const
	{
		return far_clip_;
	}
	void Camera::far_clip(const float value)
	{
		far_clip_ = value;
		update_proj_mat();
	}
	float Camera::view_angle() const
	{
		return view_angle_;
	}
	void Camera::view_angle(const float value)
	{
		view_angle_ = value;
		update_proj_mat();
	}

	void Camera::update_proj_mat()
	{
		// プロジェクション マトリックス設定
		D3DXMatrixPerspectiveFovLH(&projection_mat, D3DXToRadian(view_angle()), aspect_ratio, near_clip(), far_clip());
	}

}
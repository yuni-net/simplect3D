#pragma once

#include "popular.h"

namespace si3
{
	class camera
	{
	public:
		bool init(LPDIRECT3DDEVICE9 dxdevice, unsigned int screen_width, unsigned int screen_height);
		void update();

		void x(float value);
		float x() const;
		void y(float value);
		float y() const;
		void z(float value);
		float z() const;

		void look_x(float value);
		float look_x() const;
		void look_y(float value);
		float look_y() const;
		void look_z(float value);
		float look_z() const;

		void up_x(float value);
		float up_x() const;
		void up_y(float value);
		float up_y() const;
		void up_z(float value);
		float up_z() const;

		void radian_x(float value);
		float radian_x() const;
		void radian_y(float value);
		float radian_y() const;
		void radian_z(float value);
		float radian_z() const;

		const D3DXMATRIX & get_projection_mat() const;
		const D3DXMATRIX & view_mat() const;
		const D3DXMATRIX & view_rot() const;

		float near_clip() const;




	private:
		D3DXMATRIX	projection_mat;	// プロジェクション マトリックス
		D3DXVECTOR3	camera_coor;	// 視点座標
		D3DXVECTOR3	look_coor;		// 注視点座標
		D3DXVECTOR3	up_vector;		// 上方向ベクトル
		D3DXVECTOR3 radian;			// 回転角(ラジアン)
		LPDIRECT3DDEVICE9 dxdevice;
		D3DXMATRIX	view_rot_;
		D3DXMATRIX	view_mat_;
		float near_clip_;
	};
}
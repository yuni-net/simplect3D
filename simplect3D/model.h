#pragma once

#include "popular.h"
#include "utility.h"
#include "model_coor.h"
#include "model_data.h"
#include "display_object.h"
#include "rot_mode.h"

namespace si3
{

	class Manager;

	class model :public display_object
	{
	public:
		model(const Manager & manageri, const TCHAR * path);
		model(const Manager & manageri, const tstring & path);
		model(const Manager & manageri, const si3::model_data & modeld);
		model(const Manager & manageri, const TCHAR * path, const coor3 & center);

		bool load(const Manager & manageri, const TCHAR * path);
		bool load(const Manager & manageri, const tstring & path);
		void model_data(const Manager & manageri, const si3::model_data & modeld);
		bool load(const Manager & manageri, const TCHAR * path, const coor3 & center);


		const si3::model_data & model_data() const;

		void x(float value);
		float x() const;

		void y(float value);
		float y() const;

		void z(float value);
		float z() const;


		void radian_x(float value);
		float radian_x() const;
		
		void radian_y(float value);
		float radian_y() const;

		void radian_z(float value);
		float radian_z() const;

		void affine(const matrix & world_mat);
		const matrix & affine() const;

		void culling(bool bCulling);

		/*
		 * x,y,zの回転の順序を設定する。
		 * デフォルトでは y→x→z の順
		 */
		void set_rot_mode(rot_mode::ERotMode first, rot_mode::ERotMode second, rot_mode::ERotMode third);


		model();
		~model();

		// @override
		void draw_no_alpha() const;

		// @override
		void draw_alpha() const;
	private:
		LPDIRECT3DDEVICE9 dxdevice;
		bool ownership;
		const si3::model_data * modeld;
		model_coor coor;
		mutable matrix world_mat;
		rot_mode::ERotMode first_rot_mode;
		rot_mode::ERotMode second_rot_mode;
		rot_mode::ERotMode third_rot_mode;
		mutable bool bComputedWorldMat;
		bool bCulling;

		void construct();
		void release();
		void compute_world_mat() const;
		void set_rot_mat(matrix & rot_mat, rot_mode::ERotMode rot_mode_) const;
	};
}
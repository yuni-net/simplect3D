#pragma once

#include "si3_popular.h"
#include "si3_utility.h"
#include "si3_ModelCoor.h"
#include "si3_ModelData.h"
#include "si3_DisplayObject.h"
#include "si3_RotMode.h"

namespace si3
{

	class Manager;

	class Model :public DisplayObject
	{
	public:
		Model(const Manager & manageri, const TCHAR * path);
		Model(const Manager & manageri, const tstring & path);
		Model(const Manager & manageri, const si3::ModelData & modeld);
		Model(const Manager & manageri, const TCHAR * path, const Coor3 & center);

		bool load(const Manager & manageri, const TCHAR * path);
		bool load(const Manager & manageri, const tstring & path);
		void model_data(const Manager & manageri, const si3::ModelData & modeld);
		bool load(const Manager & manageri, const TCHAR * path, const Coor3 & center);


		const si3::ModelData & model_data() const;

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

		void affine(const Matrix & world_mat);
		const Matrix & affine() const;

		void culling(bool enable);

		/*
		 * x,y,zの回転の順序を設定する。
		 * デフォルトでは y→x→z の順
		 */
		void set_rot_mode(RotMode::ERotMode first, RotMode::ERotMode second, RotMode::ERotMode third);


		Model();
		~Model();

		// @override
		void draw_no_alpha() const;

		// @override
		void draw_alpha() const;
	private:
		LPDIRECT3DDEVICE9 dxdevice;
		bool ownership;
		const si3::ModelData * modeld;
		ModelCoor coor;
		mutable Matrix world_mat;
		RotMode::ERotMode first_rot_mode;
		RotMode::ERotMode second_rot_mode;
		RotMode::ERotMode third_rot_mode;
		mutable bool computed_world_mat;
		bool is_culling_on;

		void construct();
		void release();
		void compute_world_mat() const;
		void set_rot_mat(Matrix & rot_mat, RotMode::ERotMode rot_mode_) const;
	};
}
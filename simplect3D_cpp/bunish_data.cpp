#include <bunish_data.h>

namespace si3
{
	void bunish_data::set_local_pos(uint index, const coor3 & dust)
	{
		local_pos_list[index] = dust;
	}

	void bunish_data::set_first_pos(const coor3 & value)
	{
		pos = value;
	}

	void bunish_data::set_bunish_vec(const coor3 & value)
	{
		bunish_vec = value;
	}

	void bunish_data::set_rot_speed(const coor3 & value)
	{
		rot_speed = value;
	}

	D3DVECTOR bunish_data::compute_local_pos(uint index) const
	{
		matrix pos_mat;
		pos_mat.x(local_pos_list[index].x);
		pos_mat.y(local_pos_list[index].y);
		pos_mat.z(local_pos_list[index].z);

		pos_mat *= world_mat;

		D3DVECTOR local_pos;
		local_pos.x = pos_mat.x();
		local_pos.y = pos_mat.y();
		local_pos.z = pos_mat.z();

		return local_pos;
	}

	bool bunish_data::operator<(const bunish_data & another) const
	{
		if (another.z< this->z)	// ç~èáÇ…É\Å[ÉgÇ≥ÇπÇÈÇΩÇﬂ
		{
			return true;
		}

		return false;
	}


	void bunish_data::bunish(const si3::manager & si3m)
	{
		pos += bunish_vec;
		radian += rot_speed;
		compute_world_mat();
		compute_z(si3m);
	}

	void bunish_data::compute_world_mat()
	{
		world_mat = matrix().rotate_x(radian.x);
		world_mat *= matrix().rotate_y(radian.y);
		world_mat *= matrix().rotate_z(radian.z);
		world_mat *= matrix().parallel(pos.x, pos.y, pos.z);
	}

	void bunish_data::compute_z(const si3::manager & si3m)
	{
		// reference http://marupeke296.com/DXG_No43_ZBufferTexture.html

		matrix local_pos;
		local_pos.x(pos.x);
		local_pos.y(pos.y);
		local_pos.z(pos.z);

		matrix world_pos = local_pos * world_mat;

		const auto & view_mat = si3m.camera().view_mat();
		matrix view_pos = world_pos * view_mat;

		const auto & projection_mat = si3m.camera().get_projection_mat();
		matrix cartesian_coor = view_pos * projection_mat;

		z = cartesian_coor.z() / cartesian_coor.w();
	}


	bunish_data::bunish_data()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;

		radian.x = 0.0f;
		radian.y = 0.0f;
		radian.z = 0.0f;
	}

}
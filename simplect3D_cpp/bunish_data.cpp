#include <si3_BunishData.h>

namespace si3
{
	void BunishData::set_local_pos(uint index, const Coor3 & dust)
	{
		local_pos_list[index] = dust;
	}

	void BunishData::set_first_pos(const Coor3 & value)
	{
		pos = value;
	}

	void BunishData::set_bunish_vec(const Coor3 & value)
	{
		bunish_vec = value;
	}

	void BunishData::set_rot_speed(const Coor3 & value)
	{
		rot_speed = value;
	}

	D3DVECTOR BunishData::compute_local_pos(uint index) const
	{
		Matrix pos_mat;
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

	bool BunishData::operator<(const BunishData & another) const
	{
		if (another.z< this->z)	// ç~èáÇ…É\Å[ÉgÇ≥ÇπÇÈÇΩÇﬂ
		{
			return true;
		}

		return false;
	}


	void BunishData::bunish()
	{
		pos += bunish_vec;
		radian += rot_speed;
		compute_world_mat();
#ifdef NDEBUG
		compute_z();
#endif
	}

	void BunishData::compute_world_mat()
	{
		world_mat = Matrix().rotate_x(radian.x);
		world_mat *= Matrix().rotate_y(radian.y);
		world_mat *= Matrix().rotate_z(radian.z);
		world_mat *= Matrix().parallel(pos.x, pos.y, pos.z);
	}

	void BunishData::compute_z()
	{
		// reference http://marupeke296.com/DXG_No43_ZBufferTexture.html

		Matrix local_pos;
		local_pos.x(pos.x);
		local_pos.y(pos.y);
		local_pos.z(pos.z);

		Matrix world_pos = local_pos * world_mat;

		const auto & view_mat = si3::Manager::camera().view_mat();
		Matrix view_pos = world_pos * view_mat;

		const auto & projection_mat = si3::Manager::camera().get_projection_mat();
		Matrix cartesian_coor = view_pos * projection_mat;

		z = cartesian_coor.z() / cartesian_coor.w();
	}


	BunishData::BunishData()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;

		radian.x = 0.0f;
		radian.y = 0.0f;
		radian.z = 0.0f;
	}

}
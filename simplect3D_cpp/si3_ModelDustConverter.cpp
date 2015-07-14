#include <si3_ModelDustConverter.h>

namespace si3
{
	void ModelDustConverter::init(DxTop * planebuff, unsigned short * indexbuff, unsigned short index)
	{
		this->planebuff = planebuff;
		this->indexbuff = indexbuff;
		this->index = index;

		D3DVECTOR pos0 = get_plane_pos(0);
		D3DVECTOR pos1 = get_plane_pos(1);
		D3DVECTOR pos2 = get_plane_pos(2);
		center_pos = triangle_gravity_center(pos0, pos1, pos2);
	}


	void ModelDustConverter::set_bunish_data(bunish_data & bunishd)
	{
		bunishd.set_local_pos(0, convert_pos_to_dust(get_plane_pos(0)));
		bunishd.set_local_pos(1, convert_pos_to_dust(get_plane_pos(1)));
		bunishd.set_local_pos(2, convert_pos_to_dust(get_plane_pos(2)));

		Coor3 first_pos = center_pos;
	//	first_pos*model_mat;	// todo Œ³‚Ìmodel‚Ìworld matrix‚ð‚©‚¯‚é•K—v‚ª‚ ‚é
		bunishd.set_first_pos(first_pos);

		Coor3 bunish_vec = normalize_vector(center_pos);
		bunish_vec.x *= 0.03f;
		bunish_vec.y *= 0.03f;
		bunish_vec.z *= 0.03f;
		bunishd.set_bunish_vec(bunish_vec);

		Coor3 rot_speed;
		rot_speed.x = (fw::xrandom() % 10000)*0.16f*0.0001f + 0.06f;
		rot_speed.y = (fw::xrandom() % 10000)*0.16f*0.0001f + 0.06f;
		rot_speed.z = (fw::xrandom() % 10000)*0.16f*0.0001f + 0.06f;

		bunishd.set_rot_speed(rot_speed);
	}

	const DxTop & ModelDustConverter::get_top_data(uint offset) const
	{
		return planebuff[indexbuff[index + offset]];
	}



	Coor3 ModelDustConverter::convert_pos_to_dust(const D3DVECTOR & model_pos)
	{
		Coor3 dust;

		dust.x = model_pos.x - center_pos.x;
		dust.y = model_pos.y - center_pos.y;
		dust.z = model_pos.z - center_pos.z;

		return dust;
	}

	const D3DVECTOR & ModelDustConverter::get_plane_pos(unsigned short offset) const
	{
		return get_top_data(offset).pos;
	}

}
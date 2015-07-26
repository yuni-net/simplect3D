#include <si3_billboard.h>
#include <si3_top4.h>
#include <si3_diffuse.h>
#include <si3_animedata.h>
#include <si3_modelcoor.h>
#include <si3_Manager.h>

namespace si3
{
	Billboard::Billboard()
	{
		// Nothing
	}
	Billboard::Billboard(const Manager & manageri, const TCHAR * path)
	{
		load(manageri, path);
	}
	Billboard::Billboard(const Manager & manageri, const std::string & path)
	{
		load(manageri, path);
	}
	Billboard::Billboard(const Manager & manageri, const ::si3::ImageData & imaged)
	{
		image_data(manageri, imaged);
	}









	// @Override
	void Billboard::compute_world_mat() const
	{
		Matrix scale_mat;
		scale_mat.scale(scale(), scale(), scale());

		const D3DXMATRIX & view_rot = manager->camera().view_rot();
		Matrix inverse_rot = inverse_matrix_of_rot(view_rot);
		Matrix rot_mat;
		rot_mat.rotate_z(rot_z());
		rot_mat *= inverse_rot;

		Matrix parallel_mat;
		parallel_mat.parallel(x(), y(), z());

		world_mat = scale_mat;
		world_mat *= rot_mat;
		world_mat *= parallel_mat;
	}



}
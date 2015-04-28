#ifndef si3_ModelDustConverter_h_
#define si3_ModelDustConverter_h_

#include "utility.h"
#include "bunish_data.h"

namespace si3
{
	
	class ModelDustConverter
	{
	public:
		void init(top_type * planebuff, unsigned short * indexbuff, unsigned short index);
		void set_bunish_data(bunish_data & bunishd);
		const top_type & get_top_data(uint offset) const;


	private:
		top_type * planebuff;
		unsigned short * indexbuff;
		unsigned short index;
		coor3 center_pos;

	//	coor3 compute_triangle_center_of_gravity(const D3DVECTOR & pos0, const D3DVECTOR & pos1, const D3DVECTOR & pos2);
		coor3 convert_pos_to_dust(const D3DVECTOR & model_top);
		const D3DVECTOR & get_plane_pos(unsigned short offset) const;
	};
}

#endif
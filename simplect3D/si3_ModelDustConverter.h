#ifndef si3_ModelDustConverter_h_
#define si3_ModelDustConverter_h_

#include "utility.h"
#include "bunish_data.h"

namespace si3
{
	/***
	3Dモデルの頂点を一つ一つバラバラに動かします。
	*/
	class ModelDustConverter
	{
	public:
		void init(DxTop * planebuff, unsigned short * indexbuff, unsigned short index);
		void set_bunish_data(BunishData & bunishd);
		const DxTop & get_top_data(uint offset) const;


	private:
		DxTop * planebuff;
		unsigned short * indexbuff;
		unsigned short index;
		Coor3 center_pos;

	//	Coor3 compute_triangle_center_of_gravity(const D3DVECTOR & pos0, const D3DVECTOR & pos1, const D3DVECTOR & pos2);
		Coor3 convert_pos_to_dust(const D3DVECTOR & model_top);
		const D3DVECTOR & get_plane_pos(unsigned short offset) const;
	};
}

#endif
#ifndef Top_pmd_h_
#define Top_pmd_h_

namespace si3
{
#pragma pack(push, 1)
	struct Top_pmd
	{
		float pos[3];
		float normal[3];
		float uv[2];
		unsigned short bone_num[2];
		unsigned char bone_weight;
		unsigned char edge_flag;
	};
#pragma pack(pop)
}

#endif
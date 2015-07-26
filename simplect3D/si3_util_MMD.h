#ifndef si3_util_MMD_h_
#define si3_util_MMD_h_

#include <cstdint>
#include <string>
#include "si3_Top_pmd.h"

namespace si3
{
	std::string get_text(const char * words, const int buffer_byte);

#pragma pack(push, 1)
	struct Motion
	{
		char bone_name[15];
		int32_t frame_No;
		float location[3];
		float rotation[4];
		int8_t interpolation[64];
	};
#pragma pack(pop)

}

#endif